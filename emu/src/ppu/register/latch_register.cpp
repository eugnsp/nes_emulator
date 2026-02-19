#include "emu/ppu/register/latch_register.h"

#include <utility>

namespace emu::ppu {

/** Returns latch state for the address register. */
bool latch_register::store_hi() const noexcept {
    return my_latch == latch::store_hi;
}

/** Returns latch state for the scrolling position register. */
bool latch_register::store_x() const noexcept {
    return my_latch == latch::store_x;
}

/** Toggles the latch: high byte <-> low byte, X-scroll <-> Y-scroll. */
void latch_register::toggle() noexcept {
    my_latch = latch{!std::to_underlying(my_latch)};
}

/** Resets the latch - the next address/scroll store will start from the high byte/X-scroll. */
void latch_register::reset() noexcept {
    my_latch = {};
}

} // namespace emu::ppu
