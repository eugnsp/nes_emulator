#include "emu/mapper/mapper_001_mmc1.h"

#include "emu/address.h"
#include "emu/constants.h"
#include "emu/utility/dynamic_array.h"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <span>

#include <print>

namespace emu::mapper {

mapper_001_mmc1::mapper_001_mmc1(std::vector<std::uint8_t> const& prg_rom, std::vector<std::uint8_t> const&) :
    my_prg_rom_bank0(prg_rom.begin(),                   prg_rom_bank_size),
    my_prg_rom_bank1(prg_rom.end() - prg_rom_bank_size, prg_rom_bank_size),
    my_prg_rom(prg_rom)
{
    assert(my_prg_rom.size() % prg_rom_bank_size == 0);
    assert(my_prg_rom.size() >= 2 * prg_rom_bank_size);
}

std::uint8_t mapper_001_mmc1::load_prg(abstract_address addr) const {
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

void mapper_001_mmc1::store_prg(abstract_address addr, std::uint8_t value) {
    if (addr < prg_rom_start) {
        std::uint16_t const offset = addr.to_uint() - prg_ram_start.to_uint();
        my_prg_ram[offset] = value;
    }
    else {
        if ((value & std::uint8_t{0x80}) != std::uint8_t{}) {
            my_shift_reg.reset();
        }
        else {
            auto res = my_shift_reg.push((value & std::uint8_t{0x01}) != std::uint8_t{});
            if (res.has_value()) {
                if (addr < abstract_address{0xA000})
                    store_control(*res);
                else if (addr < absolute_address{0xC000})
                    store_chr_bank0(*res);
                else if (addr < absolute_address{0xE000})
                    store_chr_bank1(*res);
                else
                    store_prg_bank(*res);
            }
        }
    }
}

std::uint8_t mapper_001_mmc1::load_chr(ppu::vram_address offset) const {
    return my_chr_ram[offset.to_uint()];
}

void mapper_001_mmc1::store_chr(ppu::vram_address offset, std::uint8_t value) {
    my_chr_ram[offset.to_uint()] = value;
}

///////////////////////////////////////////////////////////////////////////////

void mapper_001_mmc1::set_switchable_bank(std::uint8_t bank_index) {
    assert(bank_index * prg_rom_bank_size < my_prg_rom.size());
    my_prg_rom_bank0 = prg_bank_span(my_prg_rom.begin() + bank_index * prg_rom_bank_size, prg_rom_bank_size);
}

void mapper_001_mmc1::store_control(std::uint8_t value) {
    std::println("contol: {}", +value);
}

void mapper_001_mmc1::store_chr_bank0(std::uint8_t value) {
    std::println("chr0: {}", +value);
}

void mapper_001_mmc1::store_chr_bank1(std::uint8_t value) {
    std::println("chr1: {}", +value);
}

void mapper_001_mmc1::store_prg_bank(std::uint8_t value) {
    std::println("prg: {}", +value);
}

} // namespace emu::map
