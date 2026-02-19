#pragma once

#include "emu/apu/register/concepts.h"
#include "emu/utility/cyclic_counter.h"

#include <cstdint>

namespace emu::apu {

/**
 * Pulse wave and noise channel envelope unit. Generates a volume envelope that is either
 * a decreasing saw waveform with optional looping or a constant volume.
 */
class envelope_unit {
public:
    /** Returns the current output volume. */
    std::uint8_t volume() const noexcept;

    /** Configures envelope parameters from the channel's control register. */
    template<concepts::envelope_control_register ControlRegister>
    void configure(ControlRegister) noexcept;

    /** Sets the restart flag to reload the decay level and divider on the next clock. */
    void schedule_restart() noexcept;

    /** Clocks the envelope. */
    void clock() noexcept;

private:
    bool restart() noexcept;

private:
    std::uint8_t my_decay_level = 0;
    std::uint8_t my_divider = 0;

    bool my_restart_scheduled = false;
    bool my_loop = false;

    bool my_constant_volume = false;
    std::uint8_t my_parameter = 0;
};

} // namespace emu::apu
