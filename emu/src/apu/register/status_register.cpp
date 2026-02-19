#include "emu/apu/register/status_register.h"

namespace emu::apu {

/** Returns whether pulse channel 1 is enabled. */
bool status_register::is_pulse1_channel_enabled() const noexcept {
    return is_set<flags::pulse1_channel>();
}

/** Returns whether pulse channel 2 is enabled. */
bool status_register::is_pulse2_channel_enabled() const noexcept {
    return is_set<flags::pulse2_channel>();
}

/** Returns whether the triangle channel is enabled. */
bool status_register::is_triangle_channel_enabled() const noexcept {
    return is_set<flags::triangle_channel>();
}

/** Returns whether the noise channel is enabled. */
bool status_register::is_noise_channel_enabled() const noexcept {
    return is_set<flags::noise_channel>();
}

/** Sets pulse channel 1 state as active. */
void status_register::set_pulse1_channel_active(bool state) noexcept {
    set<flags::pulse1_channel>(state);
}

/** Sets pulse channel 2 state as active. */
void status_register::set_pulse2_channel_active(bool state) noexcept {
    set<flags::pulse2_channel>(state);
}

/** Sets the triangle channel state as active. */
void status_register::set_triangle_channel_active(bool state) noexcept {
    set<flags::triangle_channel>(state);
}

/** Sets the noise channel state as active. */
void status_register::set_noise_channel_active(bool state) noexcept {
    set<flags::noise_channel>(state);
}

} // namespace emu::apu
