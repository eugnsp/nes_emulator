#pragma once

#include "emu/ppu/types.h"
#include "emu/utility/bit_flags.h"

#include <cstdint>

namespace emu::ppu {

/** PPU mask register ($2001, write only) - Rendering settings. */
class mask_register : private bit_flags<std::uint8_t> {
public:
    using bit_flags::bit_flags;

    /** Returns true if background is shown in leftmost 8 pixels. */
    bool show_background_left8() const noexcept;

    /** Returns true if sprites are shown in leftmost 8 pixels. */
    bool show_sprites_left8() const noexcept;

    /** Returns true if background rendering is enabled. */
    bool show_background() const noexcept;

    /** Returns true if sprite rendering is enabled. */
    bool show_sprites() const noexcept;

    /** Returns the color effects (grayscale and RGB emphasis states). */
    color_effects_flags color_effects() const noexcept;

private:
    enum class flags : underlying_type {
        grayscale            = 0b0000'0001,  // Grayscale mode
        show_background_left = 0b0000'0010,  // Show background in leftmost 8 pixels
        show_sprites_left    = 0b0000'0100,  // Show sprites in leftmost 8 pixels
        show_background      = 0b0000'1000,  // Enable background rendering
        show_sprites         = 0b0001'0000,  // Enable sprite rendering
        emphasize_red        = 0b0010'0000,  // Emphasize red color
        emphasize_green      = 0b0100'0000,  // Emphasize green color
        emphasize_blue       = 0b1000'0000   // Emphasize blue color
    };
};

} // namespace emu::ppu
