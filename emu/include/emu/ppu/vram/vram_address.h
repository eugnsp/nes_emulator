#pragma once

#include "emu/address/address_mask.h"

#include <compare>
#include <cstddef>
#include <cstdint>

namespace emu::ppu {

/** Represents a 14-bit PPU VRAM address ($0000–$3FFF). */
class vram_address {
public:
    using underlying_type = std::uint16_t;
    using difference_type = std::int32_t;

public:
    vram_address() = default;

    /** Constructs a VRAM address from the given value masked to 14 bits. */
    explicit constexpr vram_address(underlying_type addr) noexcept : my_addr(addr & mask) {}

    /** Returns the underlying numeric value. */
    constexpr underlying_type to_uint() const noexcept {
        return my_addr;
    }

    /** Sets the low byte of the VRAM address. */
    void set_lo_byte(std::uint8_t) noexcept;

    /** Sets the high byte of the VRAM address, ignoring bits 14 and 15. */
    void set_hi_byte(std::uint8_t) noexcept;

    friend constexpr std::strong_ordering operator<=>(vram_address, vram_address) = default;

private:
    static constexpr underlying_type mask = 0x3FFF;

private:
    std::uint16_t my_addr;
};

/** Returns a VRAM address advanced by the given offset (wrapping within the 14-bit address space). */
vram_address operator+(vram_address, vram_address::difference_type) noexcept;

/** Returns the signed distance between two VRAM addresses. */
vram_address::difference_type operator-(vram_address, vram_address) noexcept;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VRAM address map constants

/** Base address of pattern table (CHR) 0 region ($0000–$0FFF). */
inline constexpr auto vram_pattern_table0_addr = vram_address{0x0000};

/** Base address of pattern table (CHR) 1 region ($1000–$1FFF). */
inline constexpr auto vram_pattern_table1_addr = vram_address{0x1000};

/** Base address of nametable region ($2000–$2FFF). */
inline constexpr auto vram_nametable_addr = vram_address{0x2000};

/** Address mask selecting the nametable region. */
inline constexpr auto vram_nametable_addr_mask = address_mask<vram_address>{0x2FFF};

/** Base address of the PPU palette region ($3F00–$3FFF). */
inline constexpr auto vram_palette_addr = vram_address{0x3F00};

} // namespace emu::ppu
