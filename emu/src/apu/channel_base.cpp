#include "emu/apu/channel_base.h"

namespace emu::apu {

/** Returns true if the channel is enabled. */
bool channel_base::is_enabled() const noexcept {
    return my_enabled;
}

/** Returns true if the length counter is non-zero (i.e. channel is not silenced by length). */
bool channel_base::is_active() const noexcept {
    return my_length_counter.is_non_zero();
}

/** Enables or disables the channel. */
void channel_base::enable(bool enabled) noexcept {
    my_enabled = enabled;
    if (!my_enabled)
        my_length_counter.reset();
}

/** Clocks the length counter. */
void channel_base::clock_length() noexcept {
    my_length_counter.decrement();
}

} // namespace emu::apu
