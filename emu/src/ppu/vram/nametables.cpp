#include "emu/ppu/vram/nametables.h"

#include "emu/ppu/types.h"
#include "emu/ppu/vram/vram_address.h"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <utility>

namespace emu::ppu {

nametables::nametables(nametable_mirroring mirroring) {
    set_mirroring(mirroring);
}

/** Reads a byte from the nametable address range. */
std::uint8_t nametables::load(vram_address addr) const noexcept {
    decode_result const nt = decode_nametable_address(addr);
    return my_base_pointers[nt.table_index][nt.offset_in_table];
}

/** Writes a byte to the nametable address range. */
void nametables::store(vram_address addr, std::uint8_t value) noexcept {
    decode_result const nt = decode_nametable_address(addr);
    my_base_pointers[nt.table_index][nt.offset_in_table] = value;
}

/** Updates the mapping of logical nametables. */
void nametables::set_mirroring(nametable_mirroring mirroring) noexcept {
    my_base_pointers = base_ptrs(mirroring);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Decodes an address in the nametable address range into a nametable index and an offset. */
auto nametables::decode_nametable_address(vram_address addr) noexcept -> decode_result {
    vram_address const mirrored_addr = addr & vram_nametable_addr_mask;
    auto const offset = mirrored_addr - vram_nametable_addr;

    decode_result nt;
    nt.table_index     = offset / nametable_size;
    nt.offset_in_table = offset % nametable_size;

    return nt;
}

/** Returns pointers to the start of each logical nametable. */
auto nametables::base_ptrs(nametable_mirroring mirroring) noexcept -> base_pointers {
    auto const ptr = [this](std::uint8_t index) {
        return my_ciram.data() + index * nametable_size;
    };

    switch (mirroring) {
        case nametable_mirroring::vertical:
            return base_pointers{ptr(0), ptr(1), ptr(0), ptr(1)};

        case nametable_mirroring::horizontal:
            return base_pointers{ptr(0), ptr(0), ptr(1), ptr(1)};

        case nametable_mirroring::one_screen_lower:
            return base_pointers{ptr(0), ptr(0), ptr(0), ptr(0)};

        case nametable_mirroring::one_screen_upper:
            return base_pointers{ptr(1), ptr(1), ptr(1), ptr(1)};

        case nametable_mirroring::four_screen:
            return base_pointers{ptr(0), ptr(1), ptr(2), ptr(3)};
    }

    assert(false && "Unreachable");
    std::unreachable();
}

} // namespace emu::ppu
