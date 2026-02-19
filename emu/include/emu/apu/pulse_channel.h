#pragma once

#include "emu/apu/channel_base.h"
#include "emu/apu/unit/envelope_unit.h"
#include "emu/apu/unit/pulse_sequencer.h"
#include "emu/apu/unit/sweep_unit.h"

#include <cstdint>
#include <print>

namespace emu::apu {

class pulse_channel : public channel_base {
public:
    /** Returns the current output of the channel. */
    std::uint8_t output() const noexcept;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Channel configuration

    /** Writes a byte to the control register ($4000/$4004). */
    void store_control(std::uint8_t) noexcept;

    /** Writes a byte to the sweep register ($4001/$4005). */
    void store_sweep(std::uint8_t) noexcept;

    /** Writes a byte to the timer low register ($4002/$4006). */
    void store_timer_lo(std::uint8_t) noexcept;

    /** Writes a byte to the timer high register ($4003/$4007). */
    void store_timer_hi(std::uint8_t) noexcept;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Channel clocks

    /** Clocks the timer. */
    void clock_timer() noexcept;

    /** Clocks the envelope unit. */
    void clock_envelope() noexcept;

    /** Clocks the sweep unit. */
	template<sweep_arithmetic_mode NegateMode>
    void clock_sweep() noexcept;

private:
    pulse_sequencer my_duty_unit;
    envelope_unit   my_envelope_unit;
    sweep_unit      my_sweep_unit;
};

} // namespace emu::apu
