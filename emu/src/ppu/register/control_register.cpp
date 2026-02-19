#include "emu/ppu/register/control_register.h"

#include "emu/ppu/types.h"
#include "emu/ppu/vram/vram_address.h"

#include <cstdint>

namespace emu::ppu {

/** Returns base nametable index. */
nametable_index control_register::base_nametable_index() const noexcept {
    return nametable_index{to_uint_masked<flags::base_nametbl_addr>()};
}

/** Returns base nametable address. */
vram_address control_register::base_nametable_addr() const noexcept {
    return vram_nametable_addr + to_uint_masked<flags::base_nametbl_addr>() * nametable_size;
}

/** Returns VRAM address increment per each PPU data register read/write. */
std::uint8_t control_register::vram_address_increment() const noexcept {
    constexpr std::uint8_t addr_increment_down   = 1;
    constexpr std::uint8_t addr_increment_across = 32;

    return is_set<flags::vram_addr_increment>() ?
        addr_increment_across : addr_increment_down;
}

/** Returns sprite pattern table index for 8x8 sprites. */
pattern_table_index control_register::sprite_pattern_table_index() const noexcept {
    return is_set<flags::sprite_pt_table_addr>() ?
        pattern_table_index::table1 : pattern_table_index::table0;
}

/** Returns background pattern table index. */
pattern_table_index control_register::bg_pattern_table_index() const noexcept {
    return is_set<flags::bg_pt_table_addr>() ?
        pattern_table_index::table1 : pattern_table_index::table0;
}

/** Returns sprite size. */
sprite_size_mode control_register::sprite_size() const noexcept {
    return is_set<flags::sprite_size>() ?
        sprite_size_mode::s8x16 : sprite_size_mode::s8x8;
}

/** Returns whether NMI is triggered at the start of the vertical blanking interval. */
bool control_register::enable_vblank_nmi() const noexcept {
    return is_set<flags::enable_vblank_nmi>();
}

} // namespace emu::ppu
