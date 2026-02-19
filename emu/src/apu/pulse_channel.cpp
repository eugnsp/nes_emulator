#include "emu/apu/pulse_channel.h"

#include "emu/apu/register/pulse_channel_registers.h"

#include <cstdint>

namespace emu::apu {

/** Returns the current output of the channel. */
std::uint8_t pulse_channel::output() const noexcept {
    if (!is_enabled() || !is_active())
        return 0;

    if (my_timer.period() < 8 || my_timer.period() > 0x7FF)
        return 0;

    if (!my_duty_unit.output())
        return 0;

    return my_envelope_unit.volume();
}

/** Writes a byte to the control register ($4000/$4004). */
void pulse_channel::store_control(std::uint8_t value) noexcept {
    pulse_control_register const reg(value);

    my_length_counter.set_halted(reg.is_length_counter_halted());
    my_duty_unit.set_duty(reg.duty());
    my_envelope_unit.configure(reg);
}

/** Writes a byte to the sweep register ($4001/$4005). */
void pulse_channel::store_sweep(std::uint8_t value) noexcept {
    pulse_sweep_register const reg(value);
    my_sweep_unit.configure(reg);
}

/** Writes a byte to the timer low register ($4002/$4006). */
void pulse_channel::store_timer_lo(std::uint8_t value) noexcept {
    pulse_timer_lo_register const reg(value);
    my_timer.set_period_lo(reg.timer_lo_byte());
}

/** Writes a byte to the timer high register ($4003/$4007). */
void pulse_channel::store_timer_hi(std::uint8_t value) noexcept {
    pulse_timer_hi_register const reg(value);
    my_timer.set_period_hi(reg.timer_hi_byte());

    my_duty_unit.reset();
    if (is_enabled())
        my_length_counter.reload(reg.length_counter());

    my_envelope_unit.schedule_restart();
}

/** Clocks the timer. */
void pulse_channel::clock_timer() noexcept {
    if (my_timer.clock())
        my_duty_unit.clock();
}

/** Clocks the envelope unit. */
void pulse_channel::clock_envelope() noexcept {
    my_envelope_unit.clock();
}

/** Clocks the sweep unit. */
template<sweep_arithmetic_mode NegateMode>
void pulse_channel::clock_sweep() noexcept {
    my_sweep_unit.clock<NegateMode>(my_timer);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template void pulse_channel::clock_sweep<sweep_arithmetic_mode::ones_complement>();
template void pulse_channel::clock_sweep<sweep_arithmetic_mode::twos_complement>();

} // namespace emu::apu
