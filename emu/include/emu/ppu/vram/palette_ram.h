#pragma once

#include "emu/address/address_mask.h"
#include "emu/ppu/oam/sprite_attributes.h"
#include "emu/ppu/types.h"
#include "emu/ppu/vram/vram_address.h"

#include <array>
#include <cstddef>
#include <cstdint>

namespace emu::ppu {

/** Palette RAM table. */
class palette_ram {
public:
    /** Reads a byte from palette RAM. */
    std::uint8_t load(vram_address, std::uint8_t open_bus) const noexcept;

    /** Writes a byte to palette RAM. */
    void store(vram_address, std::uint8_t) noexcept;

    /** Returns the system color for the given palette color index. */
    system_color_index operator[](color_index) const noexcept;

private:
    using color_index_array = std::array<system_color_index, palette_ram_size>;
    color_index_array my_color_indices = make_default_palette();

private:
    /** Returns the default palette used at power-on, before a game modifies it. */
    static color_index_array make_default_palette() noexcept;

    /** Maps a VRAM address to a palette entry index (in the range 0–31). */
    static std::uint8_t entry_index(vram_address) noexcept;
};

} // namespace emu::ppu
