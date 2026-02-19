#include "emu/ppu/register/scroll_register.h"

#include <cstdint>

namespace emu::ppu {

/** Returns the fine components of scroll byte. */
std::uint8_t scroll_register::fine() const noexcept {
    return to_uint_masked<flags::fine>();
}

/** Returns the coarse components of scroll byte. */
std::uint8_t scroll_register::coarse() const noexcept {
    return to_uint_masked<flags::coarse>();
}

} // namespace emu::ppu
