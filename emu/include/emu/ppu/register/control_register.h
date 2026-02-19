#pragma once

#include "emu/ppu/types.h"
#include "emu/utility/bit_flags.h"

#include <cstdint>

namespace emu::ppu {

/** PPU control register ($2000, write only). */
class control_register : private bit_flags<std::uint8_t> {
public:
    using bit_flags::bit_flags;

    /** Returns base nametable index. */
    nametable_index base_nametable_index() const noexcept;

    /** Returns base nametable address. */
    vram_address base_nametable_addr() const noexcept;

    /** Returns VRAM address increment per each PPU data register read/write. */
    std::uint8_t vram_address_increment() const noexcept;

    /** Returns sprite pattern table index for 8x8 sprites. */
    pattern_table_index sprite_pattern_table_index() const noexcept;

    /** Returns background pattern table index. */
    pattern_table_index bg_pattern_table_index() const noexcept;

    /** Returns sprite size. */
    sprite_size_mode sprite_size() const noexcept;

    /** Returns whether NMI is triggered at the start of the vertical blanking interval. */
    bool enable_vblank_nmi() const noexcept;

private:
    enum class flags : underlying_type {
        base_nametbl_addr    = 0b0000'0011,  // Base nametable address: 00 = $2000, 01 = $2400, 10 = $2800, 11 = $2C00
        vram_addr_increment  = 0b0000'0100,  // VRAM address increment: 0 = add 1, going across; 1 = add 32, going down
        sprite_pt_table_addr = 0b0000'1000,  // Sprite pattern table address for 8x8 sprites: 0 = $0000, 1 = $1000
        bg_pt_table_addr     = 0b0001'0000,  // Background pattern table address: 0 = $0000, 1 = $1000
        sprite_size          = 0b0010'0000,  // Sprite size: 0 = 8x8 sprite, 1 = 8x16 sprite
        enable_vblank_nmi    = 0b1000'0000   // Generate an NMI at the start of the vertical blanking interval
    };
};

} // namespace emu::ppu
