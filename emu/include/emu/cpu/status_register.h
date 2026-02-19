#pragma once

#include "emu/utility/bit_flags.h"
#include "emu/cpu/status_register_flags.h"

#include <cstdint>

namespace emu::cpu {

/** CPU status (flags) register. */
class status_register : public bit_flags<std::uint8_t> {
public:
    status_register();

    status_register& set_c_from_value(std::uint16_t) noexcept;

    status_register& set_z_from_value(std::uint8_t) noexcept;
    status_register& set_n_from_value(std::uint8_t) noexcept;
    status_register& set_zn_from_value(std::uint8_t) noexcept;

    status_register& set_v_from_value(std::uint8_t) noexcept;
    status_register& set_v_from_values(std::uint8_t a, std::uint8_t b, std::uint8_t result) noexcept;

    std::uint8_t as_byte_with_break() const noexcept;
    std::uint8_t as_byte_without_break() const noexcept;

    void set_from_byte_without_break(std::uint8_t flags) noexcept;

    bool fn() const noexcept { return is_set<flags::negative>(); }
    bool fz() const noexcept { return is_set<flags::zero    >(); }
    bool fc() const noexcept { return is_set<flags::carry   >(); }
    bool fi() const noexcept { return is_set<flags::irq     >(); }
    bool fd() const noexcept { return is_set<flags::decimal >(); }
    bool fv() const noexcept { return is_set<flags::overflow>(); }

    status_register& set_n(bool value) noexcept { set<flags::negative>(value); return *this; };
    status_register& set_z(bool value) noexcept { set<flags::zero    >(value); return *this; };
    status_register& set_c(bool value) noexcept { set<flags::carry   >(value); return *this; };
    status_register& set_i(bool value) noexcept { set<flags::irq     >(value); return *this; };
    status_register& set_d(bool value) noexcept { set<flags::decimal >(value); return *this; };
    status_register& set_v(bool value) noexcept { set<flags::overflow>(value); return *this; };

private:
    enum class flags : underlying_type {
        carry    = 0b0000'0001,  // C - Carry flag
        zero     = 0b0000'0010,  // Z - Zero flag
        irq      = 0b0000'0100,  // I - Interrupt disable flag
        decimal  = 0b0000'1000,  // D - Decimal mode (has no effect on NES)
        brk      = 0b0001'0000,  // B - Break command bit (0 - pushed by interrupts, 1 - pushed by instructions
        unused   = 0b0010'0000,  // Unused, always pushed as 1
        overflow = 0b0100'0000,  // V - Overflow flag
        negative = 0b1000'0000   // N - Negative flag
    };
};

} // namespace emu::cpu
