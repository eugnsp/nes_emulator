#include "emu/file/nes_file.h"

#include "emu/constants.h"
#include "emu/file/nes_file_flags.h"
#include "emu/mapper.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <print>

namespace emu {

struct nes_file_header {
    std::uint8_t    signature[4];
    std::uint8_t    prg_rom_banks;  // Size of PRG ROM in 16 KB units
    std::uint8_t    chr_rom_banks;  // Size of CHR ROM in 8 KB units
    nes_file_flags6 flags6;
    nes_file_flags7 flags7;
    std::uint8_t       _unused[8];

    static constexpr decltype(signature) expected_signature = {'N', 'E', 'S', 0x1A};
};

static_assert(sizeof(nes_file_header) == 16);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

nes_file read_nes_file(std::filesystem::path const& file_path) {
    std::ifstream file;
    file.exceptions(std::ios::badbit | std::ios::failbit);
    file.open(file_path, std::ios::binary);

    nes_file_header header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));

    if (!std::ranges::equal(header.signature, header.expected_signature))
        throw std::runtime_error("Bad NES file signature");

    if (header.flags7.version() != 0)
        throw std::runtime_error("Unsupported NES file version");
    if (header.flags6.has_trainer())
        throw std::runtime_error("Trainer is not supported");

    nes_file nf;

    nf.mapper = mapper::mapper_number{static_cast<std::uint8_t>(header.flags6.mapper_lo_nybble() | header.flags7.mapper_hi_nybble())};
    nf.mirroring = header.flags6.mirroring();

    std::size_t const prg_rom_size = header.prg_rom_banks * prg_rom_bank_size;
    std::size_t const chr_rom_size = header.chr_rom_banks * chr_rom_bank_size;

    nf.prg_rom.resize(prg_rom_size);
    file.read(reinterpret_cast<char*>(nf.prg_rom.data()), prg_rom_size);

    nf.chr_rom.resize(chr_rom_size);
    file.read(reinterpret_cast<char*>(nf.chr_rom.data()), chr_rom_size);

    return nf;
}

void print_nes_file_info(nes_file const& nf) {
    assert(nf.prg_rom.size() % prg_rom_bank_size == 0);
    assert(nf.chr_rom.size() % chr_rom_bank_size == 0);

    std::println("PRG ROM: {:2d} x 16KiB", nf.prg_rom.size() / prg_rom_bank_size);
    std::println("CHR ROM: {:2d} x  8KiB", nf.chr_rom.size() / chr_rom_bank_size);
    std::println("Mapper: {}", mapper::to_string(nf.mapper));
}

} // namespace emu
