#pragma once

#include "emu/address/absolute_address.h"

namespace emu::cpu {

/** Helper class to access the interrupt vector table (IVT). */
template<class Bus>
class ivt_traits {
public:
    /** Returns the non-maskable interrupt (NMI) handler address. */
    static absolute_address nmi(Bus& bus) {
        return absolute_address{bus.load_word(ivt_nmi_address)};
    }

    /** Sets the non-maskable interrupt (NMI) handler address. */
    static void set_nmi(Bus& bus, absolute_address addr) {
        bus.store_word(ivt_nmi_address, addr.to_uint());
    }

    /** Returns the start location (RESET) address. */
    static absolute_address reset(Bus& bus) {
        return absolute_address{bus.load_word(ivt_reset_address)};
    }

    /** Sets the start location (RESET) address. */
    static void set_reset(Bus& bus, absolute_address addr) {
        bus.store_word(ivt_reset_address, addr.to_uint());
    }

    /** Returns the interrupt request (IRQ) handler address. */
    static absolute_address irq(Bus& bus) {
        return absolute_address{bus.load_word(ivt_irq_address)};
    }

    /** Sets the interrupt request (IRQ) handler address. */
    static void set_irq(Bus& bus, absolute_address addr) {
        bus.store_word(ivt_irq_address, addr.to_uint());
    }

private:
    /** IVT addresses. */
    static constexpr absolute_address ivt_nmi_address   = absolute_address{0xFFFA};
    static constexpr absolute_address ivt_reset_address = absolute_address{0xFFFC};
    static constexpr absolute_address ivt_irq_address   = absolute_address{0xFFFE};
};

} // namespace emu::cpu
