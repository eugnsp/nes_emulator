#pragma once

#include "emu/cpu/addressing_mode_tags.h"
#include "emu/cpu/vcpu.h"
#include "emu/cpu/vcpu_state.h"
#include "emu/utility/functional.h"

#include <cstdint>
#include <functional>

namespace emu::cpu {

/** Effective address calculator. */
template<addressing_mode_tag, bool AddCycleOnPageCross = false>
struct effective_address;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace detail {

/** Effective address calculator for absolute addressing modes. */
template<auto OffsetFn, bool AddCycleOnPageCross = false>
struct effective_address_abs {
    template<class Bus>
    static absolute_address get(vcpu<Bus>& cpu, word_operand op) {
        absolute_address const abs_addr = absolute_address{*op};
        absolute_address const eff_addr = abs_addr + std::invoke(OffsetFn, cpu);

        if constexpr (AddCycleOnPageCross)
            if (abs_addr.page() != eff_addr.page())
                cpu.advance_cycle_counter();

        return eff_addr;
    }
};

/** Effective address calculator for indirect addressing modes. */
template<auto PreOffsetFn, auto PostOffsetFn, bool AddCycleOnPageCross = false>
struct effective_address_ind {
    template<class Bus, class Operand>
    static absolute_address get(vcpu<Bus>& cpu, Operand op) {
        same_page_address const addr = same_page_address{*op} + std::invoke(PreOffsetFn, cpu);

        absolute_address const abs_addr = absolute_address{cpu.system_bus().load_word(addr)};
        absolute_address const eff_addr = abs_addr + std::invoke(PostOffsetFn, cpu);

        if constexpr (AddCycleOnPageCross)
            if (abs_addr.page() != eff_addr.page())
                cpu.advance_cycle_counter();

        return eff_addr;
    }
};

/** Effective address calculator for zero page addressing modes. */
template<auto OffsetFn>
struct effective_address_zpg {
    template<class Bus>
    static zp_address get(vcpu<Bus> const& cpu, byte_operand op) {
        zp_address const zp_addr = zp_address{*op};
        return zp_addr + std::invoke(OffsetFn, cpu);
    }
};

} // namespace detail

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Effective address calculator for the absolute addressing mode. */
template<>
struct effective_address<addressing_mode_tags::abs> :
    detail::effective_address_abs<zero_constant> {};

/** Effective address calculator for the absolute, X-indexed addressing mode. */
template<bool AddCycleOnPageCross>
struct effective_address<addressing_mode_tags::abs_x, AddCycleOnPageCross> :
    detail::effective_address_abs<&vcpu_state::x, AddCycleOnPageCross> {};

/** Effective address calculator for the absolute, Y-indexed addressing mode. */
template<bool AddCycleOnPageCross>
struct effective_address<addressing_mode_tags::abs_y, AddCycleOnPageCross> :
    detail::effective_address_abs<&vcpu_state::y, AddCycleOnPageCross> {};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Effective address calculator for the indirect addressing mode. */
template<>
struct effective_address<addressing_mode_tags::ind> :
    detail::effective_address_ind<zero_constant, zero_constant> {};

/** Effective address calculator for the X-indexed, indirect addressing mode. */
template<>
struct effective_address<addressing_mode_tags::x_ind> :
    detail::effective_address_ind<&vcpu_state::x, zero_constant> {};

/** Effective address calculator for the indirect, Y-indexed addressing mode. */
template<bool AddCycleOnPageCross>
struct effective_address<addressing_mode_tags::ind_y, AddCycleOnPageCross> :
    detail::effective_address_ind<zero_constant, &vcpu_state::y, AddCycleOnPageCross> {};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Effective address calculator for the zero page addressing mode. */
template<>
struct effective_address<addressing_mode_tags::zpg> :
    detail::effective_address_zpg<zero_constant> {};

/** Effective address calculator for the zero page, X-indexed addressing mode. */
template<>
struct effective_address<addressing_mode_tags::zpg_x> :
    detail::effective_address_zpg<&vcpu_state::x> {};

/** Effective address calculator for the zero page, Y-indexed addressing mode. */
template<>
struct effective_address<addressing_mode_tags::zpg_y> :
    detail::effective_address_zpg<&vcpu_state::y> {};

} // namespace emu::cpu
