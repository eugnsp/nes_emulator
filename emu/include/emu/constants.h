#pragma once

#include "address.h"

#include <cstdint>

namespace emu
{


/** RAM */

/** PPU */
constexpr auto ppu_start = abstract_address{0x2000};

/** APU */
constexpr abstract_address apu_start = abstract_address{0x4000};

/* PRG RAM */
constexpr abstract_address prg_ram_start = abstract_address{0x6000};
constexpr abstract_address prg_ram_end   = abstract_address{0x8000};

/** PRG ROM */
constexpr std::uint16_t prg_rom_bank_size = 0x4000;  // = 16 KB
constexpr std::uint16_t chr_rom_bank_size = 0x2000;  // = 8 KB
constexpr abstract_address prg_rom_start = abstract_address{0x8000};

/** PRG RAM */
constexpr std::uint16_t prg_ram_size = 0x2000;

constexpr std::uint16_t ppu_oam_size = 0x0100;

} // namespace emu
