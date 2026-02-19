#pragma once

#include "../fwd.h"
#include "emu/cpu/status_register.h"
#include "emu/address.h"

#include <cstdint>

namespace emu::cpu {

class vcpu_state {
public:
    /** Resets a CPU state. */
    void reset() noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /** Registers. */

    absolute_address pc() const noexcept;
    void set_pc(absolute_address) noexcept;

    std::uint8_t a() const noexcept;
    void set_a(std::uint8_t) noexcept;

    std::uint8_t x() const noexcept;
    void set_x(std::uint8_t) noexcept;

    std::uint8_t y() const noexcept;
    void set_y(std::uint8_t) noexcept;

    std::uint8_t sp() const noexcept;
    void set_sp(std::uint8_t) noexcept;

    status_register& flags() noexcept;
    status_register const& flags() const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /** Cycle counter. */

    using cycle_counter_type = std::uint32_t;

    cycle_counter_type cycle_counter() const noexcept;
    void advance_cycle_counter(cycle_counter_type num_cycles = 1) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /** Interrupts. */

    void set_nmi_flag() noexcept;

protected:
    cycle_counter_type my_cycles = 7;       // Cycle counter

    /** Registers. */
    absolute_address my_pc;                 // Program counter register
    status_register  my_flags;              // Status (flags) register

    std::uint8_t my_a = 0;                  // Accumulator register
    std::uint8_t my_x = 0;                  // Index register X
    std::uint8_t my_y = 0;                  // Index yegister Y

    sp_address my_sp = sp_address{0xFD};    // Stack pointer register

    /** Interrupts. */
    bool my_nmi_trig_flag = false;          // NMI interrupt trigger flag
};

} // namespace emu::cpu
