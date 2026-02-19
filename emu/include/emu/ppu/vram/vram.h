#pragma once

#include "emu/ppu/vram/vram_address.h"
#include "emu/ppu/vram/palette_ram.h"
#include "emu/ppu/types.h"
#include "emu/ppu/vram/nametables.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <utility>

namespace emu::ppu {

template<class Mapper>
class vram {
public:
    vram(Mapper&, nametable_mirroring);

    /** Reads a byte from VRAM. */
    std::uint8_t load(vram_address, std::uint8_t open_bus) const;

    /** Writes a byte to VRAM. */
    void store(vram_address, std::uint8_t);

    tile_row load_tile_row(pattern_table_index, tile_index, std::uint8_t row) const;

private:
    template<class> friend class vppu_renderer; // TODO : remove
    template<class> friend class vppu;

    Mapper& my_mapper;

    nametables  my_nametables;
    palette_ram my_palette;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Mapper>
vram<Mapper>::vram(Mapper& mapper, nametable_mirroring mirroring) : my_mapper(mapper), my_nametables(mirroring) {}

template<class Mapper>
std::uint8_t vram<Mapper>::load(vram_address addr, std::uint8_t open_bus) const {
    if (addr < vram_nametable_addr)
        return my_mapper.load_chr(addr);
    else if (addr < vram_palette_addr)
        return my_nametables.load(addr);
    else
        return my_palette.load(addr, open_bus);
}

template<class Mapper>
void vram<Mapper>::store(vram_address addr, std::uint8_t value) {
    if (addr < vram_nametable_addr)
        my_mapper.store_chr(addr, value);
    else if (addr < vram_palette_addr)
        my_nametables.store(addr, value);
    else
        my_palette.store(addr, value);
}

template<class Mapper>
tile_row vram<Mapper>::load_tile_row(pattern_table_index table_idx, tile_index tile_idx, std::uint8_t row) const {
    vram_address const base_addr = vram_address(std::to_underlying(table_idx) * 0x1000);
    std::uint16_t const offset = std::to_underlying(tile_idx) * 16 + row;

    vram_address const address = base_addr + offset;
    std::uint8_t const lo_plane = my_mapper.load_chr(address);
    std::uint8_t const hi_plane = my_mapper.load_chr(address + 8);

    return tile_row{.lo = lo_plane, .hi = hi_plane};
}

} // namespace emu::ppu
