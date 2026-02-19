#pragma once

#include "emu/apu/types.h"

#include <cstdint>

namespace emu::apu {

/**
 * Waveform channels length counter. Provides automatic duration control. Once loaded with
 * a value, it can optionally count down when the length counter halt flag is clear.
 * When the counter reaches zero, the corresponding channel is silenced.
 */
class length_counter {
public:
    /** Returns true if the length counter is non-zero. */
    bool is_non_zero() const noexcept;

    /** Sets or clears the halt flag. */
    void set_halted(bool) noexcept;

    /** Resets length counter to zero. */
    void reset() noexcept;

    /** Reloads counter from the length table. */
    void reload(length_counter_index) noexcept;

    /** Decrements counter (if it is not halted and not already zero). */
    void decrement() noexcept;

private:
    bool my_halted = false;     // Length counter halt flag; when set, prevents decrementing
    std::uint8_t my_value = 0;  // Current length counter value
};

} // namespace emu::apu
