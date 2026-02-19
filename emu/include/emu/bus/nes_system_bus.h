#pragma once
#include "emu/fwd.h"

#include "emu/address.h"
#include "emu/apu/vapu.h"
#include "emu/bus/random_access_memory.h"
#include "emu/bus/bus_word_access_mixin.h"
#include "emu/mapper/concepts.h"
#include "emu/utility/bit_ops.h"

#include <cassert>
#include <cstdint>

namespace emu::bus {

/** NES system bus class, delegates CPU memory access to RAM, cartridge mapper, PPU, and APU. */
template<mapper::concepts::mapper Mapper, class PictureInit, class AudioUnit>
class nes_system_bus : public detail::bus_word_access_mixin {
public:
    nes_system_bus(random_access_memory& ram, Mapper& mapper, PictureInit& ppu, AudioUnit& apu) :
        my_ram(ram), my_mapper(mapper), my_ppu(ppu), my_apu(apu)
    {}

    ///////////////////////////////////////////////////////////////////////////
    /** Read/write operations. */

    /** Reads a byte from memory or a device register. */
    std::uint8_t load(abstract_address addr) {
        if (addr < ram_end)
            my_open_bus = my_ram.load(addr);
        else if (addr < ppu_end)
            my_open_bus = my_ppu.load(addr);
        else if (addr < apu_end)
            my_open_bus = my_apu.load(addr);
        else if (addr >= prg_ram_start)
            my_open_bus = my_mapper.load_prg(addr);

        return my_open_bus;
    }

    /** Writes a byte to memory or a device register. */
    void store(abstract_address addr, std::uint8_t value) {
        my_open_bus = value;

        if (addr < ram_end)
            my_ram.store(addr, value);
        else if (addr < ppu_end)
            my_ppu.store(addr, value);
        else if (addr == oam_dma_register)
            store_oam_dma(value);
        else if (addr < apu_end)
            my_apu.store(addr, value);
        else if (addr >= prg_ram_start)
            my_mapper.store_prg(addr, value);
    }

private:
    static constexpr auto ram_end  = abstract_address{0x2000};
    static constexpr auto ppu_end   = abstract_address{0x4000};
    static constexpr abstract_address apu_end   = abstract_address{0x4020};
    static constexpr auto oam_dma_register = abstract_address{0x4014};  // Write only

    ///////////////////////////////////////////////////////////////////////////

private:
    void store_oam_dma(std::uint8_t value) const noexcept {
        my_ppu.store_oam_data_dma(my_ram.page(page_index{value}));
    }

private:
    std::uint8_t my_open_bus = std::uint8_t{0x00};

    random_access_memory& my_ram;
    Mapper& my_mapper;

    PictureInit& my_ppu;
    AudioUnit&   my_apu;
};

} // namespace emu::bus
