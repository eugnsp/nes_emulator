#include "emu/apu/unit/envelope_unit.h"

#include "emu/apu/noise_channel.h"
#include "emu/apu/pulse_channel.h"
#include "emu/apu/register/noise_channel_registers.h"
#include "emu/apu/register/pulse_channel_registers.h"

#include <cstdint>
#include <utility>

namespace emu::apu {

/** Returns the current output volume. */
std::uint8_t envelope_unit::volume() const noexcept {
    return my_constant_volume ? my_parameter : my_decay_level;
}

/** Configures envelope parameters from the channel's control register. */
template<concepts::envelope_control_register ControlRegister>
void envelope_unit::configure(ControlRegister reg) noexcept {
    my_loop = reg.is_length_counter_halted();
    my_constant_volume = reg.has_constant_volume();
    my_parameter = reg.envelope_parameter();
}

/** Sets the restart flag to reload the decay level and divider on the next clock. */
void envelope_unit::schedule_restart() noexcept {
    my_restart_scheduled = true;
}

/** Clocks the envelope. */
void envelope_unit::clock() noexcept {
    if (restart()) [[unlikely]]
        return;

    if (my_divider > 0) [[likely]]
        --my_divider;
    else {
        my_divider = my_parameter;

        if (my_decay_level > 0) {
            --my_decay_level;
        } else if (my_loop)
            my_decay_level = 15;
    }
}

bool envelope_unit::restart() noexcept {
    if (!my_restart_scheduled) [[likely]]
        return false;

    my_decay_level = 15;
    my_divider = my_parameter;

    my_restart_scheduled = false;
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template void envelope_unit::configure(pulse_control_register);
template void envelope_unit::configure(noise_control_register);

} // namespace emu::apu
