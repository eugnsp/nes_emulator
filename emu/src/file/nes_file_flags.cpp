#include "emu/file/nes_file_flags.h"

namespace emu
{

std::uint8_t nes_file_flags6::mapper_lo_nybble() const noexcept {
    return to_uint() >> 4;
}

ppu::nametable_mirroring nes_file_flags6::mirroring() const noexcept {
    if (is_set<flags::alt_nametable_lt>())
        return ppu::nametable_mirroring::four_screen;
    else if (is_set<flags::nametable_arrgmt>())
        return ppu::nametable_mirroring::vertical;
    else
        return ppu::nametable_mirroring::horizontal;
}

bool nes_file_flags6::has_trainer() const noexcept {
    return is_set<flags::trainer>();
}

///////////////////////////////////////////////////////////////////////////////

std::uint8_t nes_file_flags7::mapper_hi_nybble() const noexcept {
    return to_uint() & 0xF0;
}

std::uint8_t nes_file_flags7::version() const noexcept {
    return (to_uint() >> 2) & 0x03;
}

///////////////////////////////////////////////////////////////////////////////

std::uint8_t mapper_number(nes_file_flags6 flags6, nes_file_flags7 flags7) {
    return flags6.mapper_lo_nybble() | flags7.mapper_hi_nybble();
}

} // namespace emu
