#pragma once

#include "emu/ppu/types.h"
#include "emu/ppu/vram/vram_address.h"
#include "emu/utility/bit_flags.h"

namespace emu::ppu {

/** Byte 1 of an OAM entry - Tile index. */
class tile_number : private bit_flags<std::uint8_t> {
public:
    using bit_flags::bit_flags;

    tile_index index_8x8() const noexcept {
        return tile_index{to_uint()};
    }

    tile_index index_8x16() const noexcept {
        return tile_index{static_cast<std::uint8_t>(2 * to_uint_masked<flags::sprite_index>())};
    }

    pattern_table_index sprite_pt_table_addr_8x16() const {
        return is_set<flags::sprite_bank>() ?
            pattern_table_index::table1 : pattern_table_index::table0;
    }

private:
    enum class flags : underlying_type {
        sprite_bank  = 0b0000'0001,  // Bank of tiles: 0 = $0000, 1 = $1000
        sprite_index = 0b1111'1110   // Tile number of top of sprite (0 to 254; bottom half gets the next tile)
    };
};

} // namespace emu::ppu
