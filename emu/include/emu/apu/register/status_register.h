#pragma once

#include "emu/utility/bit_flags.h"

#include <cstdint>

namespace emu::apu {

/**
 * APU status register ($4015, read/write).
 */
class status_register : private bit_flags<std::uint8_t> {
public:
    using bit_flags::bit_flags;
    using bit_flags::to_uint;

    /** Returns whether pulse channel 1 is enabled. */
    bool is_pulse1_channel_enabled() const noexcept;

    /** Returns whether pulse channel 2 is enabled. */
    bool is_pulse2_channel_enabled() const noexcept;

    /** Returns whether the triangle channel is enabled. */
    bool is_triangle_channel_enabled() const noexcept;

    /** Returns whether the noise channel is enabled. */
    bool is_noise_channel_enabled() const noexcept;

    /** Sets pulse channel 1 state as active. */
    void set_pulse1_channel_active(bool) noexcept;

    /** Sets pulse channel 2 state as active. */
    void set_pulse2_channel_active(bool) noexcept;

    /** Sets the triangle channel state as active. */
    void set_triangle_channel_active(bool) noexcept;

    /** Sets the noise channel state as active. */
    void set_noise_channel_active(bool) noexcept;

private:
    enum class flags : underlying_type {
        pulse1_channel   = 0b0000'0001,  // Enable/active pulse channel 1 flag
        pulse2_channel   = 0b0000'0010,  // Enable/active pulse channel 2 flag
        triangle_channel = 0b0000'0100,  // Enable/active triangle channel flag
        noise_channel    = 0b0000'1000,  // Enable/active noise channel flag
        dmc_channel      = 0b0001'0000   // Enable/active DMC channel flag
    };
};

} // namespace emu::apu
