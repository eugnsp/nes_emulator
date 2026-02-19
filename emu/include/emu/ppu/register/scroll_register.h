#pragma once

#include "emu/utility/bit_flags.h"

#include <cstdint>

namespace emu::ppu {

/** PPU scrolling position register ($2005, write only). */
class scroll_register : private bit_flags<std::uint8_t> {
public:
    using bit_flags::bit_flags;

    /** Returns the fine components of scroll byte. */
    std::uint8_t fine() const noexcept;

    /** Returns the coarse components of scroll byte. */
    std::uint8_t coarse() const noexcept;

private:
    enum class flags : underlying_type {
        fine   = 0b0000'0111,  // Fine scroll
        coarse = 0b1111'1000   // Coarse scroll
    };
};

} // namespace emu::ppu
