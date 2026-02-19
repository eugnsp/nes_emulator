#pragma once
#include "../fwd.h"

#include "emu/cpu/disassembler.h"
#include "emu/cpu/opcode_decoder.h"
#include "emu/cpu/operand.h"
#include "emu/cpu/vcpu_state.h"
#include "emu/cpu/ivt_traits.h"
#include "emu/utility/bit_ops.h"

#include <array>
#include <cstdint>
#include <format>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace emu::cpu {

template<class SystemBus>
class vcpu : public vcpu_state {
public:
    using bus_type = SystemBus;

public:
    vcpu(SystemBus& bus) : my_bus(bus) {
        set_initial_pc();
    }

    /** Resets a CPU. */
    void reset();

    void set_initial_pc() {
        set_pc(ivt::reset(my_bus));
    }

    [[noreturn]] void run();
    [[noreturn]] void run_at(absolute_address start_pc);
    cycle_counter_type step();

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /** Stack operations. */

    void push(std::uint8_t);
    void push_address(absolute_address);

    std::uint8_t pop();
    absolute_address pop_address();

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /** Subroutine hooks. */

    std::function<void(vcpu&)> subroutine_hook(absolute_address) const;
    void add_subroutine_hook(absolute_address, std::function<void(vcpu&)> hook_fn);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /** Disassembly callback. */

    struct disasm_info {
        absolute_address pc;
        std::string instr_name;
        std::string bytes;
        std::string operand;
    };

    template<instruction_tag, addressing_mode_tag AddrMode, instruction_type, class OperandAccess>
    void disasm_callback(std::uint8_t opcode, absolute_address pc, typename AddrMode::operand const&, OperandAccess const& op_access) const;

    template<class CallbackFn>
    void set_exec_callback(CallbackFn fn) {
        my_exec_callback = fn;
    }

    SystemBus& system_bus() {
        return my_bus;
    }

    SystemBus const& system_bus() const {
        return my_bus;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /** Load/store operations. */

    template<class OperandAccess> void op(instruction_tags::LDA, OperandAccess op_access) {
        my_flags.set_zn_from_value(my_a = op_access.load());
    }

    template<class OperandAccess> void op(instruction_tags::LDX, OperandAccess op_access) {
        my_flags.set_zn_from_value(my_x = op_access.load());
    }

    template<class OperandAccess> void op(instruction_tags::LDY, OperandAccess op_access) {
        my_flags.set_zn_from_value(my_y = op_access.load());
    }

    template<class OperandAccess> void op(instruction_tags::LAX, OperandAccess op_access) {
        my_flags.set_zn_from_value(my_a = my_x = op_access.load());
    }

    template<class OperandAccess> void op(instruction_tags::STA, OperandAccess op_access) {
        op_access.store(my_a);
    }

    template<class OperandAccess> void op(instruction_tags::STX, OperandAccess op_access) {
        op_access.store(my_x);
    }

    template<class OperandAccess> void op(instruction_tags::STY, OperandAccess op_access) {
        op_access.store(my_y);
    }

    template<class OperandAccess> void op(instruction_tags::SAX, OperandAccess op_access) {
        op_access.store(my_a & my_x);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /** Register transfers. */

    void op(instruction_tags::TAX) noexcept {
        my_flags.set_zn_from_value(my_x = my_a);
    }

    void op(instruction_tags::TAY) noexcept {
        my_flags.set_zn_from_value(my_y = my_a);
    }

    void op(instruction_tags::TXA) noexcept {
        my_flags.set_zn_from_value(my_a = my_x);
    }

    void op(instruction_tags::TYA) noexcept {
        my_flags.set_zn_from_value(my_a = my_y);
    }

    void op(instruction_tags::TSX) noexcept {
        my_flags.set_zn_from_value(my_x = my_sp.to_uint());
    }

    void op(instruction_tags::TXS) noexcept {
        my_sp = sp_address{my_x};
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /** Stack operations. */

    void op(instruction_tags::PHA) {
        push(my_a);
    }

    void op(instruction_tags::PHP) {
        push(my_flags.as_byte_with_break());
    }

    void op(instruction_tags::PLA) {
        my_flags.set_zn_from_value(my_a = pop());
    }

    void op(instruction_tags::PLP) {
        my_flags.set_from_byte_without_break(pop());
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /** Arithmetic. */

    template<class OperandAccess>
    void op(instruction_tags::ADC, OperandAccess const& op_access) {
        std::uint8_t const op = op_access.load();
        std::uint8_t const old_a = a();
        std::uint16_t const new_a = old_a + op + flags().fc();

        set_a(new_a);
        flags().set_c_from_value(new_a).set_v_from_values(old_a, op, new_a).set_zn_from_value(new_a);
    }

    template<class OperandAccess>
    void op(instruction_tags::SBC, OperandAccess const& op_access) {
        std::uint8_t const op = op_access.load();
        std::uint8_t const old_a = a();
        std::uint16_t const new_a = old_a - op - !flags().fc();

        set_a(new_a);
        flags().set_c_from_value(~new_a).set_v_from_values(old_a, ~op, new_a).set_zn_from_value(new_a);
    }

    template<class OperandAccess>
    void op(instruction_tags::ISB, OperandAccess const& op_access) {
        std::uint8_t const op = op_access.load() + 1;
        op_access.store(op);

        std::uint8_t const old_a = a();
        std::uint16_t const new_a = old_a - op - !flags().fc();

        set_a(new_a);
        flags().set_c_from_value(~new_a);
        flags().set_v_from_values(old_a, ~op, new_a);
        flags().set_zn_from_value(new_a);
    }

    template<class T>
    void compare(T reg_fn, std::uint8_t const op) {
        std::uint8_t const reg = std::invoke(reg_fn, this);
        flags().set_c(reg >= op).set_zn_from_value(reg - op);
    }

    template<class OperandAccess> void op(instruction_tags::CMP, OperandAccess op_access) {
        std::uint8_t const op = op_access.load();
        my_flags.set_c(my_a >= op).set_zn_from_value(my_a - op);
    }

    template<class OperandAccess> void op(instruction_tags::CPX, OperandAccess op_access) {
        compare(&vcpu_state::x, op_access.load());
    }

    template<class OperandAccess> void op(instruction_tags::CPY, OperandAccess op_access) {
        compare(&vcpu_state::y, op_access.load());
    }

    template<class OperandAccess> void op(instruction_tags::DCP, OperandAccess op_access) {
        std::uint8_t const op = op_access.load() - 1;
        op_access.store(op);

        compare(&vcpu_state::a, op);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /** Increments and decrements. */

    template<class OperandAccess> void op(instruction_tags::INC, OperandAccess op_access) {
        std::uint8_t const new_op = op_access.load() + 1;
        my_flags.set_zn_from_value(new_op);
        op_access.store(new_op);
    }

    template<class OperandAccess> void op(instruction_tags::DEC, OperandAccess op_access) {
        std::uint8_t const new_op = op_access.load() - 1;
        my_flags.set_zn_from_value(new_op);
        op_access.store(new_op);
    }

    void op(instruction_tags::INX) {
        my_flags.set_zn_from_value(++my_x);
    }

    void op(instruction_tags::DEX) {
        my_flags.set_zn_from_value(--my_x);
    }

    void op(instruction_tags::INY) {
        my_flags.set_zn_from_value(++my_y);
    }

    void op(instruction_tags::DEY) {
        my_flags.set_zn_from_value(--my_y);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /** Logical. */

    template<class OperandAccess>
    void op(instruction_tags::AND, OperandAccess const& op_access) {
        my_flags.set_zn_from_value(my_a &= op_access.load());
    }

    template<class OperandAccess>
    void op(instruction_tags::EOR, OperandAccess const& op_access) {
        my_flags.set_zn_from_value(my_a ^= op_access.load());
    }

    template<class OperandAccess>
    void op(instruction_tags::ORA, OperandAccess const& op_access) {
        my_flags.set_zn_from_value(my_a |= op_access.load());
    }

    template<class OperandAccess>
    void op(instruction_tags::BIT, OperandAccess const& op_access) {
        std::uint8_t const op = op_access.load();
        my_flags.set_z_from_value(op & my_a).set_v_from_value(op).set_n_from_value(op);
    }

    template<class OperandAccess>
    void op(instruction_tags::ANC, OperandAccess const& op_access) {
        my_a &= op_access.load();
        my_flags.set_zn_from_value(my_a).set_c(is_sign_bit_set(my_a));
    }

    template<class OperandAccess>
    void op(instruction_tags::ALR, OperandAccess op_access) {
        my_a &= op_access.load();
        auto const [new_a, new_fc] = shift9_right(my_a);
        my_flags.set_zn_from_value(new_a).set_c(new_fc);
        my_a = new_a;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /** Bit shifts. */

    template<class OperandAccess>
    void op(instruction_tags::ASL, OperandAccess op_access) {
        auto const [new_fc, new_op] = shift9_left(op_access.load());
        my_flags.set_c(new_fc).set_zn_from_value(new_op);
        op_access.store(new_op);
    }

    template<class OperandAccess>
    void op(instruction_tags::LSR, OperandAccess op_access) {
        auto const [new_op, new_fc] = shift9_right(op_access.load());
        my_flags.set_c(new_fc).set_zn_from_value(new_op);
        op_access.store(new_op);
    }

    template<class OperandAccess>
    void op(instruction_tags::ROL, OperandAccess op_access) {
        auto const [new_fc, new_op] = rotate9_left(op_access.load(), my_flags.fc());
        my_flags.set_c(new_fc).set_zn_from_value(new_op);
        op_access.store(new_op);
    }

    template<class OperandAccess>
    void op(instruction_tags::ROR, OperandAccess op_access) {
        auto const [new_op, new_fc] = rotate9_right(my_flags.fc(), op_access.load());
        my_flags.set_c(new_fc).set_zn_from_value(new_op);
        op_access.store(new_op);
    }

    template<class OperandAccess>
    void op(instruction_tags::SLO, OperandAccess op_access) {
        auto const [new_fc, new_op] = shift9_left(op_access.load());
        my_flags.set_c(new_fc).set_zn_from_value(my_a |= new_op);
        op_access.store(new_op);
    }

    template<class OperandAccess>
    void op(instruction_tags::SRE, OperandAccess op_access) {
        auto const [new_op, new_fc] = shift9_right(op_access.load());
        my_flags.set_c(new_fc).set_zn_from_value(my_a ^= new_op);
        op_access.store(new_op);
    }

    template<class OperandAccess>
    void op(instruction_tags::RLA, OperandAccess op_access) {
        auto const [new_fc, new_op] = rotate9_left(op_access.load(), flags().fc());
        my_flags.set_c(new_fc).set_zn_from_value(my_a &= new_op);
        op_access.store(new_op);
    }

    template<class RotateFn>
    std::uint8_t rotate_add(RotateFn fn, std::uint8_t const op) {
        auto const [new_op, new_fc] = std::invoke(fn, flags().fc(), op);

        std::uint8_t const old_a = a();
        std::uint16_t const new_a = old_a + new_op + new_fc;

        set_a(new_a);
        flags().set_c_from_value(new_a);
        flags().set_v_from_values(old_a, new_op, new_a);
        flags().set_zn_from_value(new_a);

        return new_op;
    }

    template<class OperandAccess> void op(instruction_tags::RRA, OperandAccess op_access) {
        op_access.store(rotate_add(&rotate9_right, op_access.load()));
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /** Jumps and calls. */

    template<class OperandAccess>
    void op(instruction_tags::JSR, OperandAccess const& op_access) {
        absolute_address const new_pc = absolute_address(op_access.get());

        std::function<void(vcpu<SystemBus>&)> const hook = subroutine_hook(new_pc);
        if (hook)
            hook(*this);
        else {
            absolute_address const pc_m1 = pc() - 1;

            push_address(pc_m1);

            // my_bus.load(pc() + 2);
            set_pc(new_pc);
        }
    }

    void op(instruction_tags::RTS) {
        set_pc(pop_address() + 1);
    }

    template<class OperandAccess>
    void op(instruction_tags::JMP, OperandAccess const& op_access) {
        my_pc = absolute_address(op_access.get());
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /** Branches. */

    void op_branch_impl(std::int8_t offset) {
        absolute_address const old_pc = std::exchange(my_pc, my_pc + offset);
        advance_cycle_counter(old_pc.page() == my_pc.page() ? 1 : 2);
    }

    template<class OperandAccess> void op(instruction_tags::BCC, OperandAccess op_access) {
        if (!my_flags.fc())
            op_branch_impl(op_access.get());
    }

    template<class OperandAccess> void op(instruction_tags::BCS, OperandAccess op_access) {
        if (my_flags.fc())
            op_branch_impl(op_access.get());
    }

    template<class OperandAccess> void op(instruction_tags::BNE, OperandAccess op_access) {
        if (!my_flags.fz())
            op_branch_impl(op_access.get());
    }

    template<class OperandAccess> void op(instruction_tags::BEQ, OperandAccess op_access) {
        if (my_flags.fz())
            op_branch_impl(op_access.get());
    }

    template<class OperandAccess> void op(instruction_tags::BPL, OperandAccess op_access) {
        if (!my_flags.fn())
            op_branch_impl(op_access.get());
    }

    template<class OperandAccess> void op(instruction_tags::BMI, OperandAccess op_access) {
        if (my_flags.fn())
            op_branch_impl(op_access.get());
    }

    template<class OperandAccess> void op(instruction_tags::BVC, OperandAccess op_access) {
        if (!my_flags.fv())
            op_branch_impl(op_access.get());
    }

    template<class OperandAccess> void op(instruction_tags::BVS, OperandAccess op_access) {
        if (my_flags.fv())
            op_branch_impl(op_access.get());
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /** Status flag changes. */

    void op(instruction_tags::CLC) {
        my_flags.set_c(false);
    }

    void op(instruction_tags::SEC) {
        my_flags.set_c(true);
    }

    void op(instruction_tags::CLD) {
        my_flags.set_d(false);
    }

    void op(instruction_tags::SED) {
        my_flags.set_d(true);
    }

    void op(instruction_tags::CLI) {
        my_flags.set_i(false);
    }

    void op(instruction_tags::SEI) {
        my_flags.set_i(true);
    }

    void op(instruction_tags::CLV) {
        my_flags.set_v(false);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /** System functions. */

    void op(instruction_tags::BRK) {
        push_address(my_pc + 1);
        push(my_flags.as_byte_with_break());

        my_flags.set_i(true);
        my_pc = ivt_traits<SystemBus>::irq(system_bus());
    }

    void op(instruction_tags::RTI) {
        my_flags.set_from_byte_without_break(pop());
        my_pc = pop_address();
        // cpu.my_nmi_trig_flag = false;
    }

    void op(instruction_tags::NOP) {
    }

    template<class OperandAccess>
    void op(instruction_tags::NOP, OperandAccess op_access) {
        op_access.load();  // Dummy load
    }

public:
    SystemBus& my_bus;

    /** Instruction execution callback */
    std::function<void(vcpu const&, disasm_info const&)> my_exec_callback;

private:
    using ivt = ivt_traits<SystemBus>;

    std::unordered_map<absolute_address, std::function<void(vcpu&)>> my_hooks;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class SystemBus>
void vcpu<SystemBus>::reset() {
    vcpu_state::reset();
    set_initial_pc();
}

template<class SystemBus>
void vcpu<SystemBus>::run() {
    reset();

    while (true) {
        absolute_address const old_pc = pc();
        step();

        // TO DO : check in jump
        if (pc() == old_pc)
            throw infinite_loop(pc());
    }
}

template<class SystemBus>
void vcpu<SystemBus>::run_at(absolute_address start_pc) {
    reset();
    my_pc = start_pc;

    while (true) {
        absolute_address const old_pc = pc();
        step();

        if (pc() == old_pc)
            throw infinite_loop(pc());
    }
}

template<class Bus>
auto vcpu<Bus>::step() -> cycle_counter_type {
    cycle_counter_type const old_cycles = my_cycles;

    if (my_nmi_trig_flag) [[unlikely]] {
        my_nmi_trig_flag = false;

        absolute_address const pc_p1 = pc();
        std::uint8_t const curr_flags = flags().as_byte_without_break();

        push_address(pc_p1);
        push(curr_flags);

        flags().set_i(true);
        set_pc(ivt::nmi(my_bus));
        advance_cycle_counter(7);

        //std::cout << "starting NMI handler, will return to: " << std::hex << pc_p1.value() << '\n';
    }

    std::uint8_t const opcode = my_bus.load(pc());
    opcode_decoder<Bus>::execute(*this, opcode);

    return my_cycles - old_cycles;
}

///////////////////////////////////////////////////////////////////////////////
/** Stack operations */

template<class SystemBus>
void vcpu<SystemBus>::push(std::uint8_t value) {
    my_bus.store(my_sp--, std::uint8_t{value});
}

template<class SystemBus>
void vcpu<SystemBus>::push_address(absolute_address addr) {
    push(get_hi_byte(addr.to_uint()));
    push(get_lo_byte(addr.to_uint()));
}

template<class SystemBus>
std::uint8_t vcpu<SystemBus>::pop() {
    return my_bus.load(++my_sp);
}

template<class SystemBus>
absolute_address vcpu<SystemBus>::pop_address() {
    std::uint8_t const value_lo = pop();
    std::uint8_t const value_hi = pop();

    return absolute_address{to_word(value_lo, value_hi)};
}

///////////////////////////////////////////////////////////////////////////////
/** Subroutine hooks */

template<class SystemBus>
std::function<void(vcpu<SystemBus>&)> vcpu<SystemBus>::subroutine_hook(absolute_address addr) const {
    auto const pos = my_hooks.find(addr);
    if (pos == my_hooks.end())
        return {};
    else
        return pos->second;
}

template<class SystemBus>
void vcpu<SystemBus>::add_subroutine_hook(absolute_address addr, std::function<void(vcpu<SystemBus>&)> hook_fn) {
    my_hooks.emplace(addr, hook_fn);
}

///////////////////////////////////////////////////////////////////////////////
/** Disassembly callback */

template<class SystemBus>
template<instruction_tag InstTag, addressing_mode_tag AddrModeTag, instruction_type InstType, class OperandAccess>
void vcpu<SystemBus>::disasm_callback(std::uint8_t opcode, absolute_address pc, typename AddrModeTag::operand const& op, OperandAccess const& op_access) const {
    if (!my_exec_callback)
        return;

    auto get_inst_name = [&](char const* name) {
        if constexpr (InstType == instruction_type::official)
            return " " + std::string(name);
        else
            return "*" + std::string(name);
    };

    disasm_info const info{
        .pc = pc,
        .instr_name = get_inst_name(InstTag::name),
        .bytes = detail::to_bytes_string<AddrModeTag>(opcode, op),
        .operand = detail::to_asm_string(AddrModeTag{}, pc, op, op_access)
    };

    my_exec_callback(*this, info);
}

} // namespace emu::cpu
