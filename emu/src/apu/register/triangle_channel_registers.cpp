#include "emu/apu/register/triangle_channel_registers.h"

namespace emu::apu {

std::uint8_t triangle_control_register::linear_counter_period() const noexcept {
    return to_uint_masked<flags::linear_cnt_load>();
}

/** Returns true if the length counter is enabled and false if halted. */
bool triangle_control_register::is_length_counter_enabled() const noexcept {
    return !is_set<flags::counter_control>();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Returns the high 3 bits of the 11-bit timer period. */
std::uint8_t triangle_timer_hi_register::timer_hi_byte() const noexcept {
    return to_uint_masked<flags::timer_hi>();
}

/** Returns the length counter reload index. */
length_counter_index triangle_timer_hi_register::length_counter() const noexcept {
    return length_counter_index{to_uint_masked<flags::length_cnt_load>()};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Returns the low 8 bits of the 11-bit timer period. */
std::uint8_t triangle_timer_lo_register::timer_lo_byte() const noexcept {
    return to_uint_masked<flags::timer_lo>();
}

} // namespace emu::apu
