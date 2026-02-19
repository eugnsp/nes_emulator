#include "emu/apu/triangle_channel.h"

#include "emu/apu/register/triangle_channel_registers.h"
#include "emu/apu/unit/triangle_sequencer.h"

#include <cstddef>
#include <cstdint>

namespace emu::apu {

/** Returns the current output of the channel. */
std::uint8_t triangle_channel::output() noexcept {
    if (!is_enabled() || !is_active() || my_timer.period() < 2)
        return 0;

    return my_sequencer.output();
}

/** Writes a byte to the control register ($4008). */
void triangle_channel::store_control(std::uint8_t value) noexcept {
    triangle_control_register const reg(value);

    my_length_counter.set_halted(!reg.is_length_counter_enabled());
    my_linear_counter.set_period(reg.linear_counter_period());
    my_linear_counter.set_control(!reg.is_length_counter_enabled());
}

/** Writes a byte to the timer low register ($400A). */
void triangle_channel::store_timer_lo(std::uint8_t value) noexcept {
    triangle_timer_lo_register const reg(value);

    my_timer.set_period_lo(reg.timer_lo_byte());
}

/** Writes a byte to the timer high register ($400B). */
void triangle_channel::store_timer_hi(std::uint8_t value) noexcept {
    triangle_timer_hi_register const reg(value);

    my_timer.set_period_hi(reg.timer_hi_byte());
    my_linear_counter.schedule_reload();
    if (is_enabled())
        my_length_counter.reload(reg.length_counter());
}

/** Clocks the timer. */
void triangle_channel::clock_timer() {
    if (my_timer.clock() && my_length_counter.is_non_zero() && my_linear_counter.is_non_zero())
		my_sequencer.clock();
}

/** Clocks the linear counter. */
void triangle_channel::clock_linear_counter() noexcept {
    my_linear_counter.clock();
}

} // namespace emu::apu
