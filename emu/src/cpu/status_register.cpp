#include "emu/cpu/status_register.h"

#include "emu/cpu/vcpu.h"
#include "emu/utility/bit_ops.h"

#include <cstdint>

namespace emu::cpu {

status_register::status_register() : bit_flags(bitwise_or(flags::unused, flags::irq)) {
}

status_register& status_register::set_c_from_value(std::uint16_t value) noexcept {
    set<flags::carry>(is_carry_out_bit_set(value));
    return *this;
}

status_register& status_register::set_z_from_value(std::uint8_t value) noexcept {
    set<flags::zero>(value == 0);
    return *this;
}

status_register& status_register::set_n_from_value(std::uint8_t value) noexcept {
    set<flags::negative>(is_sign_bit_set(value));
    return *this;
}

status_register& status_register::set_zn_from_value(std::uint8_t value) noexcept {
    set_z_from_value(value);
    set_n_from_value(value);
    return *this;
}

status_register& status_register::set_v_from_value(std::uint8_t value) noexcept {
    set<flags::overflow>((value & 0x40) != 0);
    return *this;
}

status_register& status_register::set_v_from_values(std::uint8_t a, std::uint8_t b, std::uint8_t result) noexcept {
    set<flags::overflow>(is_sign_bit_set(a) == is_sign_bit_set(b) && is_sign_bit_set(a) != is_sign_bit_set(result));
    return *this;
}

std::uint8_t status_register::as_byte_with_break() const noexcept {
    return auto(*this).set<flags::brk>(true).to_uint();
}

std::uint8_t status_register::as_byte_without_break() const noexcept {
    return auto(*this).set<flags::brk>(false).to_uint();
}

void status_register::set_from_byte_without_break(std::uint8_t flags) noexcept {
    from_uint(flags);
    set<flags::brk>(false).set<flags::unused>(true);
}

} // namespace emu::cpu
