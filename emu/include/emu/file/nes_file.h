#pragma once

#include "emu/ppu/types.h"
#include "emu/mapper/mapper_number.h"

#include <cstddef>
#include <filesystem>
#include <vector>

namespace emu {

struct nes_file {
    std::vector<std::uint8_t>   prg_rom;  // CPU program ROM data
    std::vector<std::uint8_t>   chr_rom;  // PPU character ROM data
    mapper::mapper_number       mapper;   // Mapper number used by the cartridge
    ppu::nametable_mirroring mirroring;
};

/** Reads a NES file and returns its contents. */
nes_file read_nes_file(std::filesystem::path const&);

/** Prints basic information about a NES file. */
void print_nes_file_info(nes_file const&);

} // namespace emu
