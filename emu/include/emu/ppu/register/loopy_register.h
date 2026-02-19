#pragma once

#include "emu/ppu/register/latch_register.h"
#include "emu/ppu/register/scroll_register.h"
#include "emu/ppu/types.h"
#include "emu/ppu/vram/vram_address.h"
#include "emu/utility/bit_flags.h"

#include <cstdint>
#include <cstddef>

namespace emu::ppu {

/** PPU internal "loopy" register. Holds VRAM address for both reading/writing VRAM and for fetching nametable data. */
class loopy_register : private bit_flags<std::uint16_t> {
public:
    /** Returns a 14-bit VRAM address represented by the register. */
    vram_address to_vram_address() const noexcept;

    vram_address nametable_address() const noexcept;

    /** Returns a fine Y scroll value. */
    std::uint8_t fine_y() const noexcept;

    constexpr underlying_type value() const noexcept { // TODO : remove
        return to_uint();
    }

    void set_base_nametable_index(nametable_index idx) noexcept;

    void copy_x(loopy_register const& other) noexcept;

    void copy_y(loopy_register const& other) noexcept;

    void increment_x() noexcept;
    void increment_y() noexcept;

    void increment(underlying_type n) noexcept;

    void store_address(latch_register, std::uint8_t) noexcept;
    void store_scroll(latch_register, scroll_register) noexcept;

    void set_coarse_x(std::uint8_t value) noexcept;
    void set_coarse_y(std::uint8_t value) noexcept;

    void set_fine_y(std::uint8_t value) noexcept;

    /** Sets the high byte of the register. */
    void set_hi_byte(std::uint8_t) noexcept;

    /** Sets the low byte of the register. */
    void set_lo_byte(std::uint8_t) noexcept;

private:
    enum class flags : underlying_type {
        coarse_x    = 0b000'00'00000'11111,  // Coarse X scroll
        coarse_y    = 0b000'00'11111'00000,  // Coarse Y scroll
        nametable   = 0b000'11'00000'00000,
        nametable_x = 0b000'01'00000'00000,  // Nametable X select
        nametable_y = 0b000'10'00000'00000,  // Nametable Y select
        fine_y      = 0b111'00'00000'00000,  // Fine Y scroll

        lo_byte     = 0b000'0000'1111'1111,
        hi_byte     = 0b111'1111'0000'0000
    };
};

} // namespace emu::ppu
