#pragma once

#include "emu/ppu/types.h"

#include <array>
#include <cstdint>

namespace emu::ppu {

/** Maximum number of sprites that can be rendered on a single scanline. */
inline constexpr std::uint8_t max_sprites_per_scanline = 8;

/** Struct that holds the prefetched data for a single sprite row on a scanline. */
struct scanline_sprite {
    std::uint8_t    position_x;  // Position of left side of sprite on scanline
    sprite_priority priority;    // Sprite is behind or in front of background
    bool            zero_index;  // Sprite has index 0 (for sprite 0 hit detection)

    std::array<color_index, 8> pattern;  // Pixel colors
};

} // namespace emu::ppu
