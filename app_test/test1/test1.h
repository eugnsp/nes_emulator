#pragma once

#include "emu/address/absolute_address.h"
#include "emu/bus/memory_only_bus.h"
#include "emu/cpu/vcpu.h"
#include "emu/file/bin_file.h"

#include <chrono>
#include <cstddef>
#include <exception>
#include <print>

namespace emu::test {

void run_6502_functional_test() {
    /** Execution of tests starts here (see 6502_functional_test.lst). */
    constexpr auto entry_point_addr  = absolute_address{0x0400};

    /** A trap we expect to hit upon successful test completion (see 6502_functional_test.lst). */
    constexpr auto success_trap_addr = absolute_address{0x336D};

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    using vbus = bus::memory_only_bus;
    vbus::memory_array memory;

    vbus bus(memory);
    cpu::vcpu cpu(bus);

    bus.set_ram_bytes(read_bin_file("6502_functional_test.bin"));
    auto const start_time = std::chrono::steady_clock::now();

    try {
        cpu.run_at(entry_point_addr);
        throw std::runtime_error("Reached unexpected code path");
    }
    catch (cpu::infinite_loop const& ex) {
        if (ex.address() != success_trap_addr)
            throw;
    }

    const std::chrono::duration<double, std::micro> duration = std::chrono::steady_clock::now() - start_time;
    std::println("{:.2f} Mcps", cpu.cycle_counter() / duration.count());
}

} // namespace emu::test
