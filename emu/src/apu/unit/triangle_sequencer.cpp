#include "emu/apu/unit/triangle_sequencer.h"

#include <cstdint>

namespace emu::apu {

/** Returns the current sequencer output value. */
std::uint8_t triangle_sequencer::output() const noexcept {
    constexpr std::uint8_t triangle_table[num_steps] = {
        15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    };

    return triangle_table[my_step.value()];
}

/** Advances the sequencer by one step. */
void triangle_sequencer::clock() noexcept {
    my_step.increment();
}

} // namespace emu::apu
