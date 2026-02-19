#pragma once

#include "emu/utility/cyclic_counter.h"

#include <cstdint>

namespace emu::apu {

/**
 * Triangle wave channel 32-step sequencer.
 */
class triangle_sequencer {
public:
    /** Returns the current sequencer output value. */
    std::uint8_t output() const noexcept;

    /** Advances the sequencer by one step. */
    void clock() noexcept;

private:
    static constexpr std::uint8_t num_steps = 32;
    using step_counter = cyclic_counter<std::uint8_t, num_steps>;

    step_counter my_step;
};

} // namespace emu::apu
