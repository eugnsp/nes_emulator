#pragma once

#include "emu/ppu/oam/sprite_attributes.h"
#include "emu/ppu/oam/tile_index.h"

#include <cstdint>

namespace emu::ppu {

/** Single sprite OAM (object attribute memory) entry (4 bytes). */
struct oam_entry {
    std::uint8_t      position_y;  // Y-position of top side of sprite
    tile_number        index;       // Sprite tile index
    sprite_attributes attributes;  // Sprite attribute flags
    std::uint8_t      x_position;  // X-position of left side of sprite
};

static_assert(sizeof(oam_entry) == 4);

} // namespace emu::ppu
