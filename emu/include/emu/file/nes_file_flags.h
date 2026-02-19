#pragma once

#include "emu/ppu/types.h"
#include "emu/utility/bit_flags.h"

#include <cstdint>

namespace emu {

class nes_file_flags6 : bit_flags<std::uint8_t> {
public:
    /** Returns the low nybble of a mapper number */
    std::uint8_t mapper_lo_nybble() const noexcept;

    /** Returns nametable arrangement (mirroring) */
    ppu::nametable_mirroring mirroring() const noexcept;

    /** Returns true if a trainer is present */
    bool has_trainer() const noexcept;

private:
    enum class flags : underlying_type {
        nametable_arrgmt = 0b0000'0001,  // nametable arrangement: 0 = horizontally mirrored, 1 = vertically mirrored
        batt_prg_ram     = 0b0000'0010,  // cartridge contains battery-backed PRG RAM or other persistent memory
        trainer          = 0b0000'0100,  // 512-byte trainer
        alt_nametable_lt = 0b0000'1000   // alternative nametable layout
    };
};

///////////////////////////////////////////////////////////////////////////////

class nes_file_flags7 : public bit_flags<std::uint8_t> {
public:
    /** Returns the high nybble (not shifted) of a mapper number */
    std::uint8_t mapper_hi_nybble() const noexcept;

    /** Returns a NES file version (0 = iNES). */
    std::uint8_t version() const noexcept;
};

} // namespace emu
