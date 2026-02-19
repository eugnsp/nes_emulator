#pragma once

#include <cstdint>

namespace emu {

enum class buttons : std::uint8_t {
    a      = 0b0000'0001,  // A
    b      = 0b0000'0010,  // B
    select = 0b0000'0100,  // Select
    start  = 0b0000'1000,  // Start
    up     = 0b0001'0000,  // Up arrow
    down   = 0b0010'0000,  // Down arrow
    left   = 0b0100'0000,  // Left arrow
    right  = 0b1000'0000   // Right arrow
};

} // namespace emu
