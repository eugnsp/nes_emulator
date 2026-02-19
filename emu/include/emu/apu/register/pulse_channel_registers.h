#pragma once

#include "emu/apu/register/concepts.h"
#include "emu/apu/types.h"
#include "emu/utility/bit_flags.h"

#include <cstdint>

namespace emu::apu {

/**
 * APU pulse wave channel control register ($4000/$4004, read/write).
 */
class pulse_control_register : private bit_flags<std::uint8_t> {
public:
    using bit_flags::bit_flags;

    /** Returns the duty cycle mode defining the width of the pulse. */
    duty_mode duty() const;

    /** Returns the envelope parameter (fixed volume or envelope decay rate). */
    std::uint8_t envelope_parameter() const;

    /** Returns true if volume is constant, false if envelope is used. */
    bool has_constant_volume() const;

    /** Returns true if the length counter is halted, false otherwise. */
    bool is_length_counter_halted() const noexcept;

private:
    enum class flags : underlying_type {
        envelope_parameter  = 0b0000'1111,  // Volume (if constant) or envelope decay rate
        constant_volume     = 0b0001'0000,  // 0 = envelope is used, 1 = volume is constant
        length_counter_halt = 0b0010'0000,  // 0 = length counter enabled, 1 = length counter halted, envelope loops
        duty_mode           = 0b1100'0000   // Duty cycle mode that defines the width of the pulse
    };
};

static_assert(concepts::envelope_control_register<pulse_control_register>);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * APU pulse channel sweep register ($4001/$4005, write only).
 */
class pulse_sweep_register : private bit_flags<std::uint8_t> {
public:
    using bit_flags::bit_flags;

    /** Returns the shift count (0–7). */
    std::uint8_t shift_count() const noexcept;

    /** Returns true if sweep is set to subtract mode. */
    bool is_negate() const noexcept;

    /** Returns the sweep period. */
    std::uint8_t period() const noexcept;

    /** Returns true if sweep is enabled. */
    bool is_enabled() const noexcept;

private:
    enum class flags : underlying_type {
        shift    = 0b0000'0111,  // Number of bits to shift
        negate   = 0b0000'1000,  // 0 = add to period, 1 = subtract from period
        period   = 0b0111'0000,  // Sweep divider's period
        enabled  = 0b1000'0000,  // Sweep enabled
    };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * APU pulse channel timer high register ($4003/$4007, write only).
 */
class pulse_timer_hi_register : public bit_flags<std::uint8_t> {
public:
    using bit_flags::bit_flags;

    /** Returns the high 3 bits of the 11-bit timer period. */
    std::uint8_t timer_hi_byte() const noexcept;

    /** Returns the length counter reload index. */
    length_counter_index length_counter() const noexcept;

private:
    enum class flags : underlying_type {
        timer_hi        = 0b0000'0111,  // High bits of the timer period
        linear_cnt_load = 0b1111'1000   // Length counter load index
    };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * APU pulse channel timer low register ($4002/$4006, write only).
 */
class pulse_timer_lo_register : public bit_flags<std::uint8_t> {
public:
    using bit_flags::bit_flags;

    /** Returns the low 8 bits of the 11-bit timer period. */
    std::uint8_t timer_lo_byte() const noexcept;

private:
    enum class flags : underlying_type {
        timer_lo = 0b1111'1111  // Low bits of the timer period
    };
};

} // namespace emu::apu
