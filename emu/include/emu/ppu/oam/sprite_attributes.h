#pragma once

#include "emu/ppu/types.h"
#include "emu/utility/bit_flags.h"

#include <cstdint>

namespace emu::ppu {

/** Byte 2 of an OAM entry - Sprite attributes. */
class sprite_attributes : public bit_flags<std::uint8_t> {
public:
    using bit_flags::bit_flags;

    /** Returns the sprite palette base offset in palette RAM (0x10/0x14/0x18/0x1C). */
    color_index palette() const noexcept;

    /** Returns whether the sprite appears in front of or behind the background. */
    sprite_priority priority() const noexcept;

    /** Returns true if the sprite is flipped horizontally. */
    bool flip_horz() const noexcept;

    /** Returns true if the sprite is flipped vertically. */
    bool flip_vert() const noexcept;

    /** Clears unimplemented bits that should always read as zero. */
    void clear_unimplemented_bits() noexcept;

private:
    enum class flags : underlying_type {
        palette       = 0b0000'0011,  // Palette of sprite (4 to 7)
        unimplemented = 0b0001'1100,  // Unimplemented, always read as zero
        priority      = 0b0010'0000,  // Priority: 0 = in front of background; 1 = behind background
        flip_horz     = 0b0100'0000,  // Flip sprite horizontally
        flip_vert     = 0b1000'0000   // Flip sprite vertically
    };
};

} // namespace emu::ppu
