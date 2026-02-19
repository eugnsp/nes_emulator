#pragma once

#include "emu/apu/channel_base.h"
#include "emu/apu/unit/linear_counter.h"
#include "emu/apu/unit/triangle_sequencer.h"

#include <cstddef>
#include <cstdint>

namespace emu::apu {

/**
 * Triangle wave channel.
 */
class triangle_channel : public channel_base {
public:
    /** Returns the current output of the channel. */
    std::uint8_t output() noexcept;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Channel configuration

    /** Writes a byte to the control register ($4008). */
    void store_control(std::uint8_t) noexcept;

    /** Writes a byte to the timer low register ($400A). */
    void store_timer_lo(std::uint8_t) noexcept;

    /** Writes a byte to the timer high register ($400B). */
    void store_timer_hi(std::uint8_t) noexcept;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Channel clocks

    /** Clocks the timer. */
    void clock_timer();

    /** Clocks the linear counter. */
    void clock_linear_counter() noexcept;

private:
    triangle_sequencer my_sequencer;
    linear_counter     my_linear_counter;
};

} // namespace emu::apu
