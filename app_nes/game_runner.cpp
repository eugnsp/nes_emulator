#include "game_runner.h"

#include "keyboard_controller.h"
#include "audio_stream.h"

#include "emu/apu/vapu.h"
#include "emu/bus/nes_system_bus.h"
#include "emu/bus/random_access_memory.h"
#include "emu/constants.h"
#include "emu/file/nes_file.h"
#include "emu/mapper.h"
#include "emu/ppu/frame_buffer.h"
#include "emu/ppu/vppu.h"

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

constexpr std::uint32_t nes_cpu_clock = 1'789'773;        // Cycles per second (PAL)
constexpr unsigned int const audio_sample_rate = 44'100;  // Audio frames per second
constexpr int pixel_scale = 4;

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

    using vppu = ppu::vppu<Mapper>;
    using vapu = apu::vapu;

    using system_bus = emu::bus::nes_system_bus<Mapper, vppu, vapu>;

    ppu::frame_buffer image;

    ppu::vppu<Mapper> ppu(mapper, my_nes_file.mirroring, image);

    keyboard_controller controller;
    apu::vapu apu(controller);

    random_access_memory ram;
    system_bus bus(ram, mapper, ppu, apu);

    using cpu_type = cpu::vcpu<system_bus>;
    cpu_type cpu(bus);

    ppu.set_cpu(cpu);

//#define disasm

#ifdef disasm
    cpu.set_exec_callback([&](cpu_type const& cpu, cpu_type::disasm_info const& info) {
        std::string const regs = std::format("A:{:02X} X:{:02X} Y:{:02X} P:{:02X} SP:{:02X}", cpu.a(), cpu.x(), cpu.y(), cpu.flags().to_uint(), cpu.sp());

        std::string const status = std::format("{:04X}  {:9s}{:31s}  {:s} CYC:{}", info.pc.value(), info.bytes, info.instr_name + " " + info.operand, regs, cpu.cycle_counter());
        std::cout << status << std::endl;
    });
#endif

    constexpr unsigned int window_width  = pixel_scale * ppu::frame_buffer::width;
    constexpr unsigned int window_height = pixel_scale * ppu::frame_buffer::height;

    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "NES emulator");
    window.setVerticalSyncEnabled(true);

    sf::Texture texture;
    texture.create(emu::ppu::frame_buffer::width, emu::ppu::frame_buffer::height);

    sf::Sprite sprite(texture);
    sprite.setScale(pixel_scale, pixel_scale);


    audio_stream stream(audio_sample_rate);
    stream.play();

    double const cycles_per_sample = (double)nes_cpu_clock / audio_sample_rate;
    double total_cycles = 0;

    std::size_t test_cycles = 0;
    auto test_start = std::chrono::steady_clock::now();

    std::jthread nes_thread([&](std::stop_token stop_stoken) {
        std::uint16_t cpu_synch_cycles = 0;

        while (!stop_stoken.stop_requested()) {
            auto const cpu_cycles = cpu.step();

            test_cycles += cpu_cycles;

            auto test_now = std::chrono::steady_clock::now();
            auto test_duration = test_now - test_start;
            if (test_duration > std::chrono::seconds(5)) {
                std::cout << test_cycles / std::chrono::duration_cast<std::chrono::duration<double>>(test_duration).count() << std::endl;
                test_cycles = 0;
                test_start = std::chrono::steady_clock::now();
            }

            constexpr std::uint16_t ppu_cycles_per_cpu_cycle = 3;
            for (std::uint16_t i = 0; i < cpu_cycles * ppu_cycles_per_cpu_cycle; ++i)
                ppu.step();

            apu.step(cpu_cycles);

            total_cycles += cpu_cycles;
            if (total_cycles > cycles_per_sample) {
                total_cycles -= cycles_per_sample;

                double f = apu.output();
                sf::Int16 s16 = static_cast<sf::Int16>((2*f-1) * 32767);
                stream.push(s16);
            }

            cpu_synch_cycles += cpu_cycles;
            if (cpu_synch_cycles > 4000) {
                cpu_synch_cycles -= 4000;

                // double audio_time = stream.total_samples_played / 44100.0;
                // if (emulated_time > audio_time) {
                //     double wait_sec = emulated_time - audio_time;
                //     std::cout << "Sleep: " << wait_sec * 1000 << std::endl;
                //     std::this_thread::sleep_for(std::chrono::duration<double>(wait_sec));
                //     //sleep(emulated_time - audio_time)
                // }

                std::size_t const current_fill = stream.my_fifo.size();
                if (current_fill > 770) {
                    double const excess = current_fill - 770;
                    auto const wait_sec = std::min(excess / audio_sample_rate, 10.0);

                    //std::cout << "Sleep: " << wait_sec * 1000 << std::endl;
                    std::this_thread::sleep_for(std::chrono::duration<double>(wait_sec));
                }
            }
        }
    });


    while (true) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                nes_thread.request_stop();
                nes_thread.join();

                stream.stop();
                window.close();
                return;
            }

            if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
                controller.process_keyboard_event(event);
        }

        texture.update(reinterpret_cast<sf::Uint8 const*>(image.acquire().span().data()));
        window.draw(sprite);
        window.display();
    }
}

} // namespace emu::app
