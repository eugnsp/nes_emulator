#include "emu/ppu/types.h"

#include <utility>

namespace emu::ppu {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Palettes

/** Checks whether a color index value is within the valid range (for assertions). */
bool is_valid(system_color_index idx) noexcept {
    return std::to_underlying(idx) < system_palette_size;
}

/** Checks whether a color index value is within the valid range (for assertions). */
bool is_valid(color_index idx) noexcept {
    return std::to_underlying(idx) < palette_ram_size;
}

bool is_transparent(color_index idx) noexcept {
    std::uint8_t const entry_idx = std::to_underlying(idx) & 0b0011;
    return entry_idx == 0;
}

} // namespace emu::ppu
