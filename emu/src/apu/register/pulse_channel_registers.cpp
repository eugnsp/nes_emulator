#include "emu/apu/register/pulse_channel_registers.h"

#include "emu/utility/bit_ops.h"

namespace emu::apu {

/** Returns the duty cycle mode defining the width of the pulse. */
duty_mode pulse_control_register::duty() const {
    return duty_mode{to_uint_masked<flags::duty_mode>()};
}

/** Returns the envelope parameter (fixed volume or envelope decay rate). */
std::uint8_t pulse_control_register::envelope_parameter() const {
    return to_uint_masked<flags::envelope_parameter>();
}

/** Returns true if volume is constant, false if envelope is used. */
bool pulse_control_register::has_constant_volume() const {
    return is_set<flags::constant_volume>();
}

/** Returns true if the length counter is halted, false otherwise. */
bool pulse_control_register::is_length_counter_halted() const noexcept {
    return is_set<flags::length_counter_halt>();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Returns the shift count (0–7). */
std::uint8_t pulse_sweep_register::shift_count() const noexcept {
    return to_uint_masked<flags::shift>();
}

/** Returns true if sweep is set to subtract mode. */
bool pulse_sweep_register::is_negate() const noexcept {
    return is_set<flags::negate>();
}

/** Returns the sweep period. */
std::uint8_t pulse_sweep_register::period() const noexcept {
    return to_uint_masked<flags::period>();
}

/** Returns true if sweep is enabled. */
bool pulse_sweep_register::is_enabled() const noexcept {
    return is_set<flags::enabled>();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Returns the high 3 bits of the 11-bit timer period. */
std::uint8_t pulse_timer_hi_register::timer_hi_byte() const noexcept {
    return to_uint_masked<flags::timer_hi>();
}

/** Returns the length counter reload index. */
length_counter_index pulse_timer_hi_register::length_counter() const noexcept {
    return length_counter_index{to_uint_masked<flags::linear_cnt_load>()};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Returns the low 8 bits of the 11-bit timer period. */
std::uint8_t pulse_timer_lo_register::timer_lo_byte() const noexcept {
    return to_uint_masked<flags::timer_lo>();
}

} // namespace emu::apu
