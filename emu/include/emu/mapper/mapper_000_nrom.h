#pragma once

#include "emu/address.h"
#include "emu/constants.h"
#include "emu/mapper/mapper_number.h"
#include "emu/ppu/vram/vram_address.h"

#include <cstddef>
#include <cstdint>
#include <span>

namespace emu::mapper {

/* NROM mapper (mapper 000) class. */
class mapper_000_nrom {
public:
    mapper_000_nrom(std::vector<std::uint8_t> const& prg_rom, std::vector<std::uint8_t> const& chr_rom);

    std::uint8_t load_prg(abstract_address) const;
    void store_prg(abstract_address, std::uint8_t) const;

    std::uint8_t load_chr(ppu::vram_address offset) const;
    void store_chr(ppu::vram_address offset, std::uint8_t value);

protected:
    static constexpr abstract_address prg_rom_start = abstract_address{0x8000};  // Read

protected:
    std::vector<std::uint8_t> const& my_prg_rom;
    std::vector<std::uint8_t> my_chr_rom;

    bool my_chr_ram = false;
};

} // namespace emu::map
