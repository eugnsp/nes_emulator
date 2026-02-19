#pragma once

#include "emu/utility/bit_ops.h"

#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <ranges>

namespace emu::ppu {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations

class vram_address;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// System palette

/** Total number of colors in the PPU system palette. */
inline constexpr std::uint8_t system_palette_size = 64;

/** Color index in the system palette (6-bit value, valid range: 0-63). */
enum class system_color_index : std::uint8_t {
    grayscale_mask = 0x30  // AND mask used to force a color index into the grayscale region
};

/** Checks whether a system color index value is within the valid range (for assertions). */
bool is_valid(system_color_index) noexcept;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Palette RAM

/** Total number of color entries in the palette RAM. */
inline constexpr std::uint8_t palette_ram_size = 0x20;

/** Color index in the palette RAM (5-bit value, valid range: 0-31, background: 0-15, sprites: 16-31). */
enum class color_index : std::uint8_t {
    backdrop    = 0x00,  // Color that is used when no background or sprite pixel is visible
    sprite_mask = 0x10
};

/** Checks whether a color index value is within the valid range (for assertions). */
bool is_valid(color_index) noexcept;

bool is_transparent(color_index) noexcept;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Rendering

/** Flag that determines whether sprite pixels appear behind or in front of the background. */
enum class sprite_priority {
    in_front,  // In front of background
    behind     // Behind background
};

/** Controls grayscale and RGB emphasis effects; read from the mask register. */
struct color_effects_flags {
    bool grayscale;
    bool emphasize_red;
    bool emphasize_green;
    bool emphasize_blue;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline constexpr std::uint8_t  num_nametables = 4;
inline constexpr std::uint16_t nametable_size = 0x0400;  // 1KB

enum class nametable_index : std::uint8_t {};

enum class pattern_table_index : std::uint8_t {
    table0 = 0,
    table1 = 1
};

enum class tile_index : std::uint8_t {};

/** One 8-pixel row of a tile. */
struct tile_row {
    std::uint8_t lo;  // Low bit plane (controls bit 0 of a pixel's color index)
    std::uint8_t hi;  // High bit plane (controls bit 1 of a pixel's color index)

    void decode(color_index palette, std::span<color_index, 8> dest, bool reverse = false) const {
        std::uint8_t lo_copy = lo;
        std::uint8_t hi_copy = hi;

        for (int i = 0; i < 8; ++i) {
            color_index clr = color_index{static_cast<std::uint8_t>(((hi_copy & 0x01) << 1) | (lo_copy & 0x01))};
		    lo_copy >>= 1;
		    hi_copy >>= 1;
		    if (reverse)
                dest[7 - i] = bitwise_or(palette, clr);
            else
                dest[i] = bitwise_or(palette, clr);
        }
    }
};

/** Defines the mapping of PPU nametables to physical memory. */
enum class nametable_mirroring {
    vertical,          // Maps nametables 0 and 2 to page 0; 1 and 3 to page 1
    horizontal,        // Maps nametables 0 and 1 to page 0; 2 and 3 to page 1
    one_screen_lower,  // Maps all nametables to page 0
    one_screen_upper,  // Maps all nametables to page 1
    four_screen        // Maps nametables 0–3 to pages 0-3, respectively
};

/** Sprite size, 8x8 or 8x16. */
enum class sprite_size_mode : std::uint8_t { s8x8, s8x16 };

inline constexpr std::uint8_t sprite_height(sprite_size_mode mode) {
    switch (mode) {
        case sprite_size_mode::s8x8:  return 8;
        case sprite_size_mode::s8x16: return 16;
    }
    assert(false && "Unreachable");
    std::unreachable();
}

} // namespace emu::ppu
