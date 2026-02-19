#include "emu/apu/unit/pulse_sequencer.h"

#include <cassert>
#include <cstdint>
#include <utility>

namespace emu::apu {

/** Returns the current duty output bit. */
bool pulse_sequencer::output() const noexcept {
    constexpr std::uint8_t duty_table[num_modes][num_steps] = {
        {0, 0, 0, 0, 0, 0, 0, 1},  // 12.5%
        {0, 0, 0, 0, 0, 0, 1, 1},  // 25%
        {0, 0, 0, 0, 1, 1, 1, 1},  // 50%
        {1, 1, 1, 1, 1, 1, 0, 0}   // 25% negated
    };

    return duty_table[std::to_underlying(my_mode)][my_step.value()];
}

/** Sets the duty pattern. */
void pulse_sequencer::set_duty(duty_mode mode) noexcept {
    assert(std::to_underlying(mode) < num_modes && "Duty mode index is out-of-bounds");
    my_mode = mode;
}

/** Resets the sequencer to index 0. */
void pulse_sequencer::reset() noexcept {
    my_step.reset();
}

/** Advances the sequencer by one step. */
void pulse_sequencer::clock() noexcept {
    my_step.increment();
}

} // namespace emu::apu
