#include "emu/apu/unit/length_counter.h"

#include <cassert>
#include <cstdint>
#include <utility>

namespace emu::apu {

/** Returns true if the length counter is non-zero. */
bool length_counter::is_non_zero() const noexcept {
    return my_value > 0;
}

/** Sets or clears the halt flag. */
void length_counter::set_halted(bool halted) noexcept {
    my_halted = halted;
}

/** Resets length counter to zero. */
void length_counter::reset() noexcept {
    my_value = 0;
}

/** Reloads counter from the length table. */
void length_counter::reload(length_counter_index index) noexcept {
    constexpr std::uint8_t length_table[32] = {
        10, 254, 20, 2, 40, 4, 80, 6, 160, 8, 60, 10, 14, 12, 26, 14,
        12, 16, 24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30
    };

    assert(is_valid(index) && "Length counter index is out-of-bounds");
    my_value = length_table[std::to_underlying(index)];
}

/** Decrements counter (if it is not halted and not already zero). */
void length_counter::decrement() noexcept {
    if (!my_halted && my_value > 0)
        --my_value;
}

} // namespace emu::apu
