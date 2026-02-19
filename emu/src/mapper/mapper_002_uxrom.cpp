#include "emu/mapper/mapper_002_uxrom.h"

#include "emu/address.h"
#include "emu/constants.h"
#include "emu/utility/dynamic_array.h"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <span>

namespace emu::mapper {

mapper_002_uxrom::mapper_002_uxrom(std::vector<std::uint8_t> const& prg_rom, std::vector<std::uint8_t> const&) :
    my_prg_rom_bank0(prg_rom.begin(),                   prg_rom_bank_size),
    my_prg_rom_bank1(prg_rom.end() - prg_rom_bank_size, prg_rom_bank_size),
    my_prg_rom(prg_rom)
{
    assert(my_prg_rom.size() % prg_rom_bank_size == 0);
    assert(my_prg_rom.size() >= 2 * prg_rom_bank_size);
}

std::uint8_t mapper_002_uxrom::load_prg(abstract_address addr) const {
    if (addr < prg_rom_start) {
        std::uint16_t const offset = addr.to_uint() - prg_ram_start.to_uint();
        return my_prg_ram[offset];
    }
    else {
        std::uint16_t const offset = addr.to_uint() - prg_rom_start.to_uint();
        if (offset < prg_rom_bank_size)
            return my_prg_rom_bank0[offset];
        else
            return my_prg_rom_bank1[offset - prg_rom_bank_size];
    }
}

void mapper_002_uxrom::store_prg(abstract_address addr, std::uint8_t value) {
    if (addr < prg_rom_start) {
        std::uint16_t const offset = addr.to_uint() - prg_ram_start.to_uint();
        my_prg_ram[offset] = value;
    }
    else {
        std::uint8_t const bank_index = value & 0x07;
        set_switchable_bank(bank_index);
    }
}

std::uint8_t mapper_002_uxrom::load_chr(ppu::vram_address offset) const {
    return my_chr_ram[offset.to_uint()];
}

void mapper_002_uxrom::store_chr(ppu::vram_address offset, std::uint8_t value) {
    my_chr_ram[offset.to_uint()] = value;
}

///////////////////////////////////////////////////////////////////////////////

void mapper_002_uxrom::set_switchable_bank(std::uint8_t bank_index) {
    assert(bank_index * prg_rom_bank_size < my_prg_rom.size());
    my_prg_rom_bank0 = prg_bank_span(my_prg_rom.begin() + bank_index * prg_rom_bank_size, prg_rom_bank_size);
}

} // namespace emu::map
