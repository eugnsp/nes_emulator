#pragma once

#include "emu/apu/types.h"
#include "emu/utility/cyclic_counter.h"

#include <cstdint>

namespace emu::apu {

/**
 * Pulse wave channel 8-step duty pattern sequencer.
 */
class pulse_sequencer {
public:
    /** Returns the current duty output bit. */
    bool output() const noexcept;

    /** Sets the duty pattern. */
    void set_duty(duty_mode mode) noexcept;

    /** Resets the sequencer to index 0. */
    void reset() noexcept;

    /** Advances the sequencer by one step. */
    void clock() noexcept;

private:
    static constexpr std::uint8_t num_modes = 4;
    static constexpr std::uint8_t num_steps = 8;
    using step_counter = cyclic_counter<std::uint8_t, num_steps>;

    step_counter my_step;
    duty_mode    my_mode = {};
};

} // namespace emu::apu
