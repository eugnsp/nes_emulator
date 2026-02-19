#pragma once

#include "emu/apu/register/concepts.h"
#include "emu/apu/types.h"
#include "emu/apu/unit/noise_lfsr.h"
#include "emu/utility/bit_flags.h"

#include <cstdint>

namespace emu::apu {

/**
 * APU noise channel control register ($400C, write only).
 */
class noise_control_register : private bit_flags<std::uint8_t> {
public:
    using bit_flags::bit_flags;

    /** Returns the envelope parameter (fixed volume or envelope decay rate). */
    std::uint8_t envelope_parameter() const noexcept;

    /** Returns true if volume is constant, false if envelope is used. */
    bool has_constant_volume() const noexcept;

    /** Returns true if the length counter is halted, false otherwise. */
    bool is_length_counter_halted() const noexcept;

private:
    enum class flags : underlying_type {
        envelope_parameter   = 0b0000'1111,  // Volume (if constant) or envelope parameter (decay rate)
        constant_volume      = 0b0001'0000,  // 0 = envelope is used, 1 = volume is constant
        length_counter_halt  = 0b0010'0000,  // 0 = length counter enabled, 1 = length counter halted, envelope loops
    };
};

static_assert(concepts::envelope_control_register<noise_control_register>);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * APU noise channel timer register ($400E, write only).
 */
class noise_timer_register : private bit_flags<std::uint8_t> {
public:
    using bit_flags::bit_flags;

    /** Returns the noise timer period index. */
    noise_period_index period_index() const noexcept;

    /** Returns the LFSR feedback mode. */
    noise_lfsr_mode mode() const noexcept;

private:
    enum class flags : underlying_type {
        period_index = 0b0000'1111,  // Noise timer period index
        lfsr_mode    = 0b1000'0000,  // 0 = LFSR long mode, 1 = LFSR short mode
    };
};

} // namespace emu::apu
