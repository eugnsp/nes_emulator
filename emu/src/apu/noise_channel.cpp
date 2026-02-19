#include "emu/apu/noise_channel.h"

#include "emu/utility/bit_ops.h"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <utility>

namespace emu::apu {

/** Returns the current output of the channel. */
std::uint8_t noise_channel::output() const noexcept {
    if (!is_enabled() || !is_active() || my_lfsr.lsb())
        return 0;

    return my_envelope_unit.volume();
}

/** Writes a byte to the control register ($400C). */
void noise_channel::store_control(std::uint8_t value) noexcept {
    noise_control_register const reg(value);
    my_length_counter.set_halted(reg.is_length_counter_halted());
    my_envelope_unit.configure(reg);
}

void noise_channel::store_timer(std::uint8_t value) noexcept {
    if (is_enabled())
        my_length_counter.reload(length_counter_index{static_cast<std::uint8_t>(value >> 3)});
    my_envelope_unit.schedule_restart();
}

void noise_channel::store_period(std::uint8_t value) noexcept {
    noise_timer_register const reg(value);
    my_timer.set_period(period(reg.period_index()));
    my_lfsr.set_mode(reg.mode());
}

/** Clocks the timer. */
void noise_channel::clock_timer() noexcept {
    if (my_timer.clock())
        my_lfsr.advance();
}

/** Clocks the envelope unit. */
void noise_channel::clock_envelope() noexcept {
    my_envelope_unit.clock();
}

std::uint16_t noise_channel::period(noise_period_index period_idx) noexcept {
    constexpr std::uint16_t period_table[16] = {
	    4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068,
    };

    assert(std::to_underlying(period_idx) < 16 && "Noise period index is out-of-bounds");
    return period_table[std::to_underlying(period_idx)];
}

} // namespace emu::apu
