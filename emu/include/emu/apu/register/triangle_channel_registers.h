#pragma once

#include "emu/apu/types.h"
#include "emu/utility/bit_flags.h"

namespace emu::apu {

/**
 * APU triangle channel control register ($4008, write only).
 */
class triangle_control_register : private bit_flags<std::uint8_t> {
public:
    using bit_flags::bit_flags;

    /** Returns the linear counter reload value. */
    std::uint8_t linear_counter_period() const noexcept;

    /** Returns true if the length counter is enabled and false if halted. */
    bool is_length_counter_enabled() const noexcept;

private:
    enum class flags : underlying_type {
        linear_cnt_load = 0b0111'1111,
        counter_control = 0b1000'0000
    };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * APU triangle channel timer high register ($400B, write only).
 */
class triangle_timer_hi_register : public bit_flags<std::uint8_t> {
public:
    using bit_flags::bit_flags;

    /** Returns the high 3 bits of the 11-bit timer period. */
    std::uint8_t timer_hi_byte() const noexcept;

    /** Returns the length counter reload index. */
    length_counter_index length_counter() const noexcept;

private:
    enum class flags : underlying_type {
        timer_hi        = 0b0000'0111,  // High bits of the timer period
        length_cnt_load = 0b1111'1000   // Length counter load index
    };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * APU triangle channel timer low register ($400A, write only).
 */
class triangle_timer_lo_register : public bit_flags<std::uint8_t> {
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
