#pragma once

#include "types.h"

namespace emu
{

inline constexpr uint8_t get_lo_byte(uint16_t word)
{
    return word & 0xFF;
}

inline constexpr uint8_t get_hi_byte(uint16_t word)
{
    return word >> 8;
}

inline constexpr uint16_t to_word(uint8_t byte_lo, uint8_t byte_hi)
{
    return (byte_hi << 8) | byte_lo;
}

} // namespace emu
