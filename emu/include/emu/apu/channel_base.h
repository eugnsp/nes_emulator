#pragma once

#include "emu/apu/unit/channel_timer.h"
#include "emu/apu/unit/length_counter.h"

namespace emu::apu {

/**
 * Base class for sequencer-driven waveform generator channels.
 */
class channel_base {
public:
    /** Returns true if the channel is enabled. */
    bool is_enabled() const noexcept;

    /** Returns true if the length counter is non-zero (i.e. channel is not silenced by length). */
    bool is_active() const noexcept;

    /** Enables or disables the channel. */
    void enable(bool) noexcept;

    /** Clocks the length counter. */
    void clock_length() noexcept;

protected:
    channel_base() = default;

protected:
    bool my_enabled = false;

    channel_timer  my_timer;
    length_counter my_length_counter;
};

} // namespace emu::apu
