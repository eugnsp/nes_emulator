#pragma once

#include "unit_stubs.h"

#include "emu/bus/nes_system_bus.h"
#include "emu/constants.h"
#include "emu/cpu/vcpu.h"
#include "emu/file/bin_file.h"
#include "emu/file/nes_file.h"
#include "emu/mapper/mapper_000_nrom.h"

#include <cstdint>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <print>
#include <ranges>
#include <set>
#include <string>
#include <vector>

namespace emu::test {

std::vector<std::string> read_lines(std::filesystem::path const& path) {
    std::vector<std::string> lines;

    std::ifstream file(path);
    for (std::string line; std::getline(file, line); )
        lines.push_back(std::move(line));

    return lines;
}

bool compare_logs(std::vector<std::string> const& log1, std::vector<std::string> const& log2) {
    if (log1.size() != log2.size()) {
        std::println("Numbers of lines don't match: {} - {}", log1.size(), log2.size());
        return false;
    }

    for (auto const& [line1, line2] : std::views::zip(log1, log2)) {
        if (line1.size() != line2.size()) {
            std::println("Lines don't match ({} - {})", line1.size(), line2.size());
            std::println("  {}", line1);
            std::println("  {}", line2);
            return false;
        }

        std::size_t num_mismatches = 0;
        for (auto [ch1, ch2] : std::views::zip(line1, line2))
            num_mismatches += (ch1 != ch2);

        if (num_mismatches > 15) {
        std::println("Lines don't match ({})", num_mismatches);
            std::println("  {}", line1);
            std::println("  {}", line2);
            return false;
        }
    }

    return true;
}

struct terminate_test {};

void run_nes_cpu_test() {
    emu::nes_file nf = emu::read_nes_file("nestest.nes");
    emu::random_access_memory ram;

    if (nf.prg_rom.size() == emu::prg_rom_bank_size) {
        nf.prg_rom.resize(2 * emu::prg_rom_bank_size);
        auto last = nf.prg_rom.begin() + emu::prg_rom_bank_size;
        std::copy(nf.prg_rom.begin(), last, last);
    }

    using system_bus = emu::bus::nes_system_bus<emu::mapper::mapper_000_nrom, vppu_stub, vapu_stub>;
    using vcpu = emu::cpu::vcpu<system_bus>;

    emu::mapper::mapper_000_nrom mapper(nf.prg_rom, nf.chr_rom);

    vppu_stub ppu_stub;
    vapu_stub apu_stub;

    vcpu::bus_type bus(ram, mapper, ppu_stub, apu_stub);
    vcpu cpu(bus);

    std::vector<std::string> execution_log;
    cpu.set_exec_callback([&](vcpu const& cpu, vcpu::disasm_info const& info) {
        std::string const regs = std::format("A:{:02X} X:{:02X} Y:{:02X} P:{:02X} SP:{:02X}", cpu.a(), cpu.x(), cpu.y(), cpu.flags().to_uint(), cpu.sp());

        std::string status = std::format("{:04X}  {:9s}{:31s}  {:s} CYC:{}", info.pc.to_uint(), info.bytes, info.instr_name + " " + info.operand, regs, cpu.cycle_counter());
        execution_log.push_back(std::move(status));

        if (cpu.cycle_counter() >= 26554)
            throw terminate_test();
    });

    try {
        cpu.run_at(emu::absolute_address{0xC000});
    }
    catch (terminate_test const&) {
    }

    /*if (false)*/ {
        std::ofstream file("nestest_actual.log");
        for (auto& s : execution_log)
            file << s << '\n';
    }

    std::vector const expected_log = read_lines("nestest.log");

    if (!compare_logs(execution_log, expected_log))
        throw std::runtime_error("Test failed");
}

} // namespace emu::test
