#include "emu/apu/register/noise_channel_registers.h"

#include <cassert>
#include <cstdint>

namespace emu::apu {

/** Returns the envelope parameter (fixed volume or envelope decay rate). */
std::uint8_t noise_control_register::envelope_parameter() const noexcept {
    return to_uint_masked<flags::envelope_parameter>();
}

/** Returns true if volume is constant, false if envelope is used. */
bool noise_control_register::has_constant_volume() const noexcept {
    return is_set<flags::constant_volume>();
}

/** Returns true if the length counter is halted, false otherwise. */
bool noise_control_register::is_length_counter_halted() const noexcept {
    return is_set<flags::length_counter_halt>();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Returns the noise timer period index. */
noise_period_index noise_timer_register::period_index() const noexcept {
    return noise_period_index{to_uint_masked<flags::period_index>()};
}

/** Returns the LFSR feedback mode. */
noise_lfsr_mode noise_timer_register::mode() const noexcept {
    return is_set<flags::lfsr_mode>() ?
        noise_lfsr_mode::shortened : noise_lfsr_mode::normal;
}

} // namespace emu::apu
