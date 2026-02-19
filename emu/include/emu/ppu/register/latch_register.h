#pragma once

namespace emu::ppu {

/** PPU internal write latch register. Latch is shared between the address and the scrolling position registers. */
class latch_register {
public:
    /** Returns latch state for the address register. */
    bool store_hi() const noexcept;

    /** Returns latch state for the scrolling position register. */
    bool store_x() const noexcept;

    /** Toggles the latch: high byte <-> low byte, X-scroll <-> Y-scroll. */
    void toggle() noexcept;

    /** Resets the latch - the next address/scroll store will start from the high byte/X-scroll. */
    void reset() noexcept;

private:
    enum class latch : bool {
        // Address register latch
        store_hi = false,
        store_lo = true,

        // Scrolling position register latch
        store_x = false,
        store_y = true
    };

private:
    latch my_latch = {};
};

} // namespace emu::ppu
