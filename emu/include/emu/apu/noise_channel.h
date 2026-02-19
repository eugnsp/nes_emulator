#pragma once

#include "emu/apu/channel_base.h"
#include "emu/apu/register/noise_channel_registers.h"
#include "emu/apu/unit/envelope_unit.h"
#include "emu/apu/unit/noise_lfsr.h"

#include <cstdint>
#include <cstddef>

namespace emu::apu {

class noise_channel : public channel_base {
public:
    /** Returns the current output of the channel. */
    std::uint8_t output() const noexcept;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Channel configuration

    /** Writes a byte to the control register ($400C). */
    void store_control(std::uint8_t) noexcept;

    void store_timer(std::uint8_t) noexcept;
    void store_period(std::uint8_t) noexcept;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Channel clocks

    /** Clocks the timer. */
    void clock_timer() noexcept;

    /** Clocks the envelope unit. */
    void clock_envelope() noexcept;

private:
    static std::uint16_t period(noise_period_index) noexcept;

private:
    noise_lfsr    my_lfsr;
    envelope_unit my_envelope_unit;
};

} // namespace emu::apu
