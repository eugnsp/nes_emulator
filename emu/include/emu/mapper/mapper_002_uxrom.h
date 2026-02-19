#pragma once

#include "emu/address.h"
#include "emu/constants.h"
#include "emu/mapper/mapper_number.h"
#include "emu/ppu/vram/vram_address.h"
#include "emu/utility/dynamic_array.h"

#include <cstddef>
#include <cstdint>
#include <span>

namespace emu::mapper {

/* UxROM mapper (mapper 002) class. */
class mapper_002_uxrom {
public:
    mapper_002_uxrom(std::vector<std::uint8_t> const& prg_rom, std::vector<std::uint8_t> const&);

    std::uint8_t load_prg(abstract_address) const;
    void store_prg(abstract_address, std::uint8_t value);

    std::uint8_t load_chr(ppu::vram_address offset) const;
    void store_chr(ppu::vram_address offset, std::uint8_t value);

private:
    void set_switchable_bank(std::uint8_t bank_index);

private:
    static constexpr abstract_address prg_ram_start = abstract_address{0x6000};  // Read/Write
    static constexpr abstract_address prg_rom_start = abstract_address{0x8000};  // Read/Write
    static constexpr std::uint16_t prg_ram_size = prg_rom_start.to_uint() - prg_ram_start.to_uint();

    using prg_bank_span = std::span<std::uint8_t const, prg_rom_bank_size>;

    prg_bank_span       my_prg_rom_bank0;  // $8000-$BFFF, 16 KB switchable bank
    prg_bank_span const my_prg_rom_bank1;  // $C000-$FFFF, 16 KB fixed bank

    dynamic_array<std::uint8_t, prg_ram_size>      my_prg_ram;
    dynamic_array<std::uint8_t, chr_rom_bank_size> my_chr_ram;

    std::vector<std::uint8_t> const& my_prg_rom;
};

} // namespace emu::map
