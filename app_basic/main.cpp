#include "ncurses_io.h"

#include "emu/address/absolute_address.h"
#include "emu/bus/memory_only_bus.h"
#include "emu/cpu/vcpu.h"
#include "emu/file/bin_file.h"

#include <cstddef>
#include <exception>
#include <print>

namespace emu::basic {

void run_microtan_basic() {
    /** Execution of BASIC ROM starts here. */
    constexpr auto entry_point_addr = absolute_address{0xE07A};

    /** IO subroutines addresses. */
    constexpr auto input_subroutine_addr  = absolute_address{0xE210};
    constexpr auto output_subroutine_addr = absolute_address{0xE216};

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    using vbus = bus::memory_only_bus;
    vbus::memory_array memory;

    vbus bus(memory, 0xC000);  // $0000-$BFFF - RAM; $C000-$FFFF - ROM
    cpu::vcpu cpu(bus);

    cpu.add_subroutine_hook(input_subroutine_addr,  &ncurses_io::input);
    cpu.add_subroutine_hook(output_subroutine_addr, &ncurses_io::output);

    bus.set_rom_bytes(read_bin_file("microtan.bin"));
    cpu.run_at(entry_point_addr);
}

} // namespace emu::basic

int main() {
    ncurses_io::init();

    try {
        emu::basic::run_microtan_basic();
        ncurses_io::finalize();
    }
    catch (std::exception const& ex) {
        ncurses_io::finalize();
        std::println("Exception: {}", ex.what());
    }

    return 0;
}
