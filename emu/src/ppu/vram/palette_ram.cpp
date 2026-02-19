#include "emu/ppu/vram/palette_ram.h"

#include "emu/ppu/types.h"
#include "emu/ppu/vram/vram_address.h"

#include <cassert>
#include <cstddef>
#include <cstdint>

namespace emu::ppu {

/** Reads a byte from palette RAM. */
std::uint8_t palette_ram::load(vram_address addr, std::uint8_t open_bus) const noexcept {
    constexpr auto open_bus_mask = std::uint8_t{0b1100'0000};

    system_color_index const color_idx = my_color_indices[entry_index(addr)];
    return std::uint8_t{std::to_underlying(color_idx)} | (open_bus & open_bus_mask);
}

/** Writes a byte to palette RAM. */
void palette_ram::store(vram_address addr, std::uint8_t value) noexcept {
    constexpr auto color_mask = std::uint8_t{system_palette_size - 1};

    std::uint8_t const color_idx = value & color_mask;
    my_color_indices[entry_index(addr)] = system_color_index{color_idx};
}

/** Returns the system color for the given palette color index. */
system_color_index palette_ram::operator[](color_index color_idx) const noexcept {
    assert(is_valid(color_idx));
    return my_color_indices[std::to_underlying(color_idx)];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Returns the default palette used at power-on, before a game modifies it. */
palette_ram::color_index_array palette_ram::make_default_palette() noexcept {
    auto const to_color_index_array = [](auto... color_idxes) {
        static_assert(sizeof...(color_idxes) == palette_ram_size, "Bad number of entries");
        return color_index_array{static_cast<system_color_index>(color_idxes)...};
    };

    return to_color_index_array(
                          /* Palette 0 */           /* Palette 1 */           /* Palette 2 */           /* Palette 3 */
        /* Background */  0x0F, 0x00, 0x01, 0x02,   0x03, 0x04, 0x05, 0x06,   0x07, 0x08, 0x09, 0x0A,   0x0B, 0x0C, 0x0D, 0x0E,
        /* Sprite */      0x0F, 0x00, 0x01, 0x02,   0x03, 0x04, 0x05, 0x06,   0x07, 0x08, 0x09, 0x0A,   0x0B, 0x0C, 0x0D, 0x0E
    );
}

/** Maps a VRAM address to a palette entry index (in the range 0–31). */
std::uint8_t palette_ram::entry_index(vram_address addr) noexcept {
    constexpr auto entry_idx_mask = address_mask<vram_address>{palette_ram_size - 1};
    vram_address const mirrored_addr = addr & entry_idx_mask;

    std::uint8_t entry_idx = static_cast<std::uint8_t>(mirrored_addr.to_uint());

    // Map 0x10/0x14/0x18/0x1C -> 0x00/0x04/0x08/0x0C
    if ((entry_idx & 0x03) == 0)
        entry_idx &= ~0x10;

    return entry_idx;
}

} // namespace emu::ppu
