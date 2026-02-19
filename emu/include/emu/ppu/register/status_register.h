#pragma once

#include "emu/utility/bit_flags.h"

#include <cstddef>
#include <cstdint>

namespace emu::ppu {

/** PPU status register ($2002, read only). */
class status_register : private bit_flags<std::uint8_t> {
public:
    using bit_flags::to_uint;

    /** Updates the open bus bits from a given open bus value. */
    void set_open_bus_bits(std::uint8_t) noexcept;

    /** Clears the sprite overflow flag. */
    void clear_sprite_overflow() noexcept;

    /** Sets the sprite overflow flag. */
    void set_sprite_overflow() noexcept;

    /** Clears the sprite 0 hit flag. */
    void clear_sprite_zero_hit() noexcept;

    /** Sets the sprite 0 hit flag. */
    void set_sprite_zero_hit() noexcept;

    /** Returns true if the v-blank flag is set. */
    bool is_vblank_set() const noexcept;

    /** Clears the v-blank flag. */
    void clear_vblank() noexcept;

    /** Sets the v-blank flag. */
    void set_vblank() noexcept;

private:
    enum class flags : underlying_type {
        open_bus        = 0b0001'1111,  // Open bus bits
        sprite_overflow = 0b0010'0000,  // Sprite overflow flag
        sprite_zero_hit = 0b0100'0000,  // Sprite 0 hit flag
        in_vblank       = 0b1000'0000   // V-blank flag, cleared on read
    };
};

} // namespace emu::ppu
