#include "emu/mapper/mapper_000_nrom.h"

#include "emu/address.h"
#include "emu/constants.h"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

namespace emu::mapper {

mapper_000_nrom::mapper_000_nrom(std::vector<std::uint8_t> const& prg_rom, std::vector<std::uint8_t> const& chr_rom) :
    my_prg_rom(prg_rom), my_chr_rom(chr_rom)
{
    assert(my_prg_rom.size() == prg_rom_bank_size || my_prg_rom.size() == 2 * prg_rom_bank_size);
    //assert(chr_rom.size() == chr_rom_bank_size);
    if (my_chr_rom.empty()) {
        my_chr_ram = true;
        my_chr_rom.resize(chr_rom_bank_size);
    }
}

std::uint8_t mapper_000_nrom::load_prg(abstract_address addr) const {
    if (addr >= prg_rom_start) {
        std::uint16_t offset = addr.to_uint() - prg_rom_start.to_uint();
        offset &= (my_prg_rom.size() - 1);
        return my_prg_rom[offset];
    } else
        return std::uint8_t{0x00};
}

void mapper_000_nrom::store_prg(abstract_address, std::uint8_t) const {
    // ROM - do nothing
}

std::uint8_t mapper_000_nrom::load_chr(ppu::vram_address offset) const {
    return std::uint8_t{my_chr_rom[offset.to_uint()]};
}

void mapper_000_nrom::store_chr(ppu::vram_address offset, std::uint8_t value) {
    if (my_chr_ram)
        my_chr_rom[offset.to_uint()] = value;
    // ROM - do nothing
}

} // namespace emu::map
