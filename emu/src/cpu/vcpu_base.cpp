#include "emu/cpu/vcpu_state.h"

#include "emu/address/absolute_address.h"

#include <cstdint>

namespace emu::cpu {

void vcpu_state::reset() noexcept {
    *this = vcpu_state();
}

///////////////////////////////////////////////////////////////////////////////
/** Registers. */

absolute_address vcpu_state::pc() const noexcept {
    return my_pc;
}

void vcpu_state::set_pc(absolute_address value) noexcept {
    my_pc = value;
}

std::uint8_t vcpu_state::a() const noexcept {
    return my_a;
}

void vcpu_state::set_a(std::uint8_t value) noexcept {
    my_a = value;
}

std::uint8_t vcpu_state::x() const noexcept {
    return my_x;
}

void vcpu_state::set_x(std::uint8_t value) noexcept {
    my_x = value;
}

std::uint8_t vcpu_state::y() const noexcept {
    return my_y;
}

void vcpu_state::set_y(std::uint8_t value) noexcept {
    my_y = value;
}

std::uint8_t vcpu_state::sp() const noexcept {
    return my_sp.to_uint();
}

void vcpu_state::set_sp(std::uint8_t value) noexcept {
    my_sp = sp_address(value);
}

status_register& vcpu_state::flags() noexcept {
    return my_flags;
}

status_register const& vcpu_state::flags() const noexcept {
    return my_flags;
}

///////////////////////////////////////////////////////////////////////////////
/** Cycle counter. */

auto vcpu_state::cycle_counter() const noexcept -> cycle_counter_type {
    return my_cycles;
}

void vcpu_state::advance_cycle_counter(cycle_counter_type num_cycles /* = 1 */) noexcept {
    my_cycles += num_cycles;
}

///////////////////////////////////////////////////////////////////////////////
/** Interrupts. */

void vcpu_state::set_nmi_flag() noexcept {
    my_nmi_trig_flag = true;
}

} // namespace emu::cpu
