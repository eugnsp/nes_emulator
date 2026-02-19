#include "game_runner.h"

#include "audio_stream.h"
#include "bulk_queue.h"
#include "keyboard_controller.h"
#include "main_window.h"

#include "emu/apu/vapu.h"
#include "emu/bus/nes_system_bus.h"
#include "emu/bus/random_access_memory.h"
#include "emu/constants.h"
#include "emu/file/nes_file.h"
#include "emu/mapper.h"
#include "emu/ppu/frame_buffer.h"
#include "emu/ppu/vppu.h"
#include "emu/utility/cyclic_counter.h"

#include <SFML/Graphics.hpp>

#include <cstdint>
#include <exception>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <thread>

namespace emu::app {

constexpr std::uint32_t nes_cpu_clock = 1'789'773;      // Cycles per second (PAL)
constexpr std::uint32_t audio_sample_rate = 44'100;     // Audio frames per second

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

game_runner::game_runner(std::filesystem::path const& nes_file_path) {
    my_nes_file = read_nes_file(nes_file_path);
}

void game_runner::run() {
    print_nes_file_info(my_nes_file);

    auto const run_fn = [this](auto mapper_tag) { run_with_mapper(mapper_tag); };
    if (!mapper::with_mapper_type(my_nes_file.mapper, run_fn))
        throw std::runtime_error("Unsupported mapper");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Mapper>
void game_runner::run_with_mapper(std::type_identity<Mapper>) {
    Mapper mapper(my_nes_file.prg_rom, my_nes_file.chr_rom);

    using system_bus = emu::bus::nes_system_bus<Mapper, ppu::vppu<Mapper>, apu::vapu>;

    ppu::frame_buffer image;
    ppu::vppu ppu(mapper, my_nes_file.mirroring, image);

    keyboard_controller controller;
    apu::vapu apu(controller);

    random_access_memory ram;
    system_bus bus(ram, mapper, ppu, apu);

    cpu::vcpu cpu(bus);

    ppu.set_cpu(cpu);

//#define disasm

#ifdef disasm
    cpu.set_exec_callback([&](cpu_type const& cpu, cpu_type::disasm_info const& info) {
        std::string const regs = std::format("A:{:02X} X:{:02X} Y:{:02X} P:{:02X} SP:{:02X}", cpu.a(), cpu.x(), cpu.y(), cpu.flags().to_uint(), cpu.sp());

        std::string const status = std::format("{:04X}  {:9s}{:31s}  {:s} CYC:{}", info.pc.value(), info.bytes, info.instr_name + " " + info.operand, regs, cpu.cycle_counter());
        std::cout << status << std::endl;
    });
#endif

    main_window wnd;

    audio_stream stream(audio_sample_rate);
    bulk_queue<sf::Event> keyboard_events;

    std::jthread system_thread([&](std::stop_token stop_token) {
        std::array<sf::Int16, audio_stream::block_size> audio_samples_block;
        cyclic_counter<std::uint32_t, nes_cpu_clock> audio_samples_counter;

        while (!stop_token.stop_requested()) {
            keyboard_events.consume_all([&](auto& event) {
                controller.process_keyboard_event(event);
            });

            auto it = audio_samples_block.begin();
            while (it < audio_samples_block.end()) {
                auto const cpu_cycles = cpu.step();
                ppu.step(cpu_cycles);
                apu.step(cpu_cycles);

                if (audio_samples_counter.increment(cpu_cycles * audio_sample_rate))
                    *it++ = to_pcm_sample<audio_stream::value_type>(apu.output());
            }

            stream.push(audio_samples_block);  // blocks if stream buffer is full
        }
    });

    stream.wait_until_prefilled();
    stream.play();

    auto run_event_loop = [&]() {
        while (wnd.is_open()) {
            while (true) {
                std::optional const event = wnd.poll_event();
                if (!event.has_value())
                    break;

                if (event->type == sf::Event::Closed)
                    return;

                if (event->type == sf::Event::KeyPressed || event->type == sf::Event::KeyReleased)
                    keyboard_events.push(*event);
            }

            wnd.update_image(image.acquire().span());
            wnd.display();
        }
    };

    run_event_loop();

    system_thread.request_stop();
    system_thread.join();

    stream.stop();
    wnd.close();
}

} // namespace emu::app
