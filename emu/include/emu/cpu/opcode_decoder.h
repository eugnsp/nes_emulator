#pragma once

#include "emu/cpu/addressing_mode_tags.h"
#include "emu/cpu/exceptions.h"
#include "emu/cpu/fwd.h"
#include "emu/cpu/instruction_tags.h"
#include "emu/cpu/num_cycles.h"
#include "emu/cpu/operand_access.h"

#include <array>
#include <cstdint>
#include <functional>

namespace emu::cpu {

enum class instruction_type { official, unofficial };



template<class Bus>
struct opcode_decoder {
private:
    using cpu_type = vcpu<Bus>;
    using instruction_fn_ptr = void(*)(cpu_type&, std::uint8_t /*opcode */);
    using opcode_table = std::array<instruction_fn_ptr, 256>;

public:
    static void execute(cpu_type& cpu, std::uint8_t opcode);

private:
    template<instruction_tag, addressing_mode_tag, num_cycles NumCycles, instruction_type>
    static void execute(cpu_type&, std::uint8_t opcode);

    static void op_illegal(cpu_type&, std::uint8_t opcode);

    /** Official instructions */
    template<instruction_tag InstTag, addressing_mode_tag AddrModeTag, num_cycles NumCycles>
    static constexpr auto op = &execute<InstTag, AddrModeTag, NumCycles, instruction_type::official>;

    /** Unofficial instructions */
    template<instruction_tag InstTag, addressing_mode_tag AddrModeTag, num_cycles NumCycles>
    static constexpr auto up = &execute<InstTag, AddrModeTag, NumCycles, instruction_type::unofficial>;

    static constexpr opcode_table make_opcode_table() noexcept;

private:
    static constexpr auto my_opcode_table = make_opcode_table();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Bus>
void opcode_decoder<Bus>::execute(cpu_type& cpu, std::uint8_t opcode) {
    std::invoke(my_opcode_table[opcode], cpu, opcode);
}

template<class Bus>
constexpr auto opcode_decoder<Bus>::make_opcode_table() noexcept -> opcode_table {
    using namespace addressing_mode_tags;
    using namespace instruction_tags;
    using namespace page_cross_literals;

    // Suffix "_p" means +1 cycle on page cross
    return {
                   /* 0x-0*/        /* 0x-1*/             /* 0x-2*/        /* 0x-3*/             /* 0x-4*/          /* 0x-5*/          /* 0x-6*/          /* 0x-7*/          /* 0x-8*/        /* 0x-9*/             /* 0x-A*/        /* 0x-B*/          /* 0x-C*/             /* 0x-D*/             /* 0x-E*/             /* 0x-F*/
        /* 0x0- */ op<BRK, imp, 7>, op<ORA, x_ind, 6  >, op_illegal,      up<SLO, x_ind, 8  >, up<NOP, zpg,   3>, op<ORA, zpg,   3>, op<ASL, zpg,   5>, up<SLO, zpg,   5>, op<PHP, imp, 3>, op<ORA, imm,   2  >, op<ASL, acc, 2>, up<ANC, imm,   2>, up<NOP, abs,   4  >, op<ORA, abs,   4  >, op<ASL, abs,   6  >, up<SLO, abs,   6  >,
        /* 0x1- */ op<BPL, rel, 2>, op<ORA, ind_y, 5_p>, op_illegal,      up<SLO, ind_y, 8  >, up<NOP, zpg_x, 4>, op<ORA, zpg_x, 4>, op<ASL, zpg_x, 6>, up<SLO, zpg_x, 6>, op<CLC, imp, 2>, op<ORA, abs_y, 4_p>, up<NOP, imp, 2>, up<SLO, abs_y, 7>, up<NOP, abs_x, 4_p>, op<ORA, abs_x, 4_p>, op<ASL, abs_x, 7  >, up<SLO, abs_x, 7  >,
        /* 0x2- */ op<JSR, abs, 6>, op<AND, x_ind, 6  >, op_illegal,      up<RLA, x_ind, 8  >, op<BIT, zpg,   3>, op<AND, zpg,   3>, op<ROL, zpg,   5>, up<RLA, zpg,   5>, op<PLP, imp, 4>, op<AND, imm,   2  >, op<ROL, acc, 2>, up<ANC, imm,   2>, op<BIT, abs,   4  >, op<AND, abs,   4  >, op<ROL, abs,   6  >, up<RLA, abs,   6  >,
        /* 0x3- */ op<BMI, rel, 2>, op<AND, ind_y, 5_p>, op_illegal,      up<RLA, ind_y, 8  >, up<NOP, zpg_x, 4>, op<AND, zpg_x, 4>, op<ROL, zpg_x, 6>, up<RLA, zpg_x, 6>, op<SEC, imp, 2>, op<AND, abs_y, 4_p>, up<NOP, imp, 2>, up<RLA, abs_y, 7>, up<NOP, abs_x, 4_p>, op<AND, abs_x, 4_p>, op<ROL, abs_x, 7  >, up<RLA, abs_x, 7  >,
        /* 0x4- */ op<RTI, imp, 6>, op<EOR, x_ind, 6  >, op_illegal,      up<SRE, x_ind, 8  >, up<NOP, zpg,   3>, op<EOR, zpg,   3>, op<LSR, zpg,   5>, up<SRE, zpg,   5>, op<PHA, imp, 3>, op<EOR, imm,   2  >, op<LSR, acc, 2>, up<ALR, imm,   2>, op<JMP, abs,   3  >, op<EOR, abs,   4  >, op<LSR, abs,   6  >, up<SRE, abs,   6  >,
        /* 0x5- */ op<BVC, rel, 2>, op<EOR, ind_y, 5_p>, op_illegal,      up<SRE, ind_y, 8  >, up<NOP, zpg_x, 4>, op<EOR, zpg_x, 4>, op<LSR, zpg_x, 6>, up<SRE, zpg_x, 6>, op<CLI, imp, 2>, op<EOR, abs_y, 4_p>, up<NOP, imp, 2>, up<SRE, abs_y, 7>, up<NOP, abs_x, 4_p>, op<EOR, abs_x, 4_p>, op<LSR, abs_x, 7  >, up<SRE, abs_x, 7  >,
        /* 0x6- */ op<RTS, imp, 6>, op<ADC, x_ind, 6  >, op_illegal,      up<RRA, x_ind, 8  >, up<NOP, zpg,   3>, op<ADC, zpg,   3>, op<ROR, zpg,   5>, up<RRA, zpg,   5>, op<PLA, imp, 4>, op<ADC, imm,   2  >, op<ROR, acc, 2>, op_illegal,        op<JMP, ind,   5  >, op<ADC, abs,   4  >, op<ROR, abs,   6  >, up<RRA, abs,   6  >,
        /* 0x7- */ op<BVS, rel, 2>, op<ADC, ind_y, 5_p>, op_illegal,      up<RRA, ind_y, 8  >, up<NOP, zpg_x, 4>, op<ADC, zpg_x, 4>, op<ROR, zpg_x, 6>, up<RRA, zpg_x, 6>, op<SEI, imp, 2>, op<ADC, abs_y, 4_p>, up<NOP, imp, 2>, up<RRA, abs_y, 7>, up<NOP, abs_x, 4_p>, op<ADC, abs_x, 4_p>, op<ROR, abs_x, 7  >, up<RRA, abs_x, 7  >,
        /* 0x8- */ up<NOP, imm, 2>, op<STA, x_ind, 6  >, up<NOP, imm, 2>, up<SAX, x_ind, 6  >, op<STY, zpg,   3>, op<STA, zpg,   3>, op<STX, zpg,   3>, up<SAX, zpg,   3>, op<DEY, imp, 2>, up<NOP, imm,   2  >, op<TXA, imp, 2>, op_illegal,        op<STY, abs,   4  >, op<STA, abs,   4  >, op<STX, abs,   4  >, up<SAX, abs,   4  >,
        /* 0x9- */ op<BCC, rel, 2>, op<STA, ind_y, 6  >, op_illegal,      op_illegal,          op<STY, zpg_x, 4>, op<STA, zpg_x, 4>, op<STX, zpg_y, 4>, up<SAX, zpg_y, 4>, op<TYA, imp, 2>, op<STA, abs_y, 5  >, op<TXS, imp, 2>, op_illegal,        op_illegal,          op<STA, abs_x, 5  >, op_illegal,          op_illegal,
        /* 0xA- */ op<LDY, imm, 2>, op<LDA, x_ind, 6  >, op<LDX, imm, 2>, up<LAX, x_ind, 6  >, op<LDY, zpg,   3>, op<LDA, zpg,   3>, op<LDX, zpg,   3>, up<LAX, zpg,   3>, op<TAY, imp, 2>, op<LDA, imm,   2  >, op<TAX, imp, 2>, op_illegal,        op<LDY, abs,   4  >, op<LDA, abs,   4  >, op<LDX, abs,   4  >, up<LAX, abs,   4  >,
        /* 0xB- */ op<BCS, rel, 2>, op<LDA, ind_y, 5_p>, op_illegal,      up<LAX, ind_y, 5_p>, op<LDY, zpg_x, 4>, op<LDA, zpg_x, 4>, op<LDX, zpg_y, 4>, up<LAX, zpg_y, 4>, op<CLV, imp, 2>, op<LDA, abs_y, 4_p>, op<TSX, imp, 2>, op_illegal,        op<LDY, abs_x, 4_p>, op<LDA, abs_x, 4_p>, op<LDX, abs_y, 4_p>, up<LAX, abs_y, 4_p>,
        /* 0xC- */ op<CPY, imm, 2>, op<CMP, x_ind, 6  >, up<NOP, imm, 2>, up<DCP, x_ind, 8  >, op<CPY, zpg,   3>, op<CMP, zpg,   3>, op<DEC, zpg,   5>, up<DCP, zpg,   5>, op<INY, imp, 2>, op<CMP, imm,   2  >, op<DEX, imp, 2>, op_illegal,        op<CPY, abs,   4  >, op<CMP, abs,   4  >, op<DEC, abs,   6  >, up<DCP, abs,   6  >,
        /* 0xD- */ op<BNE, rel, 2>, op<CMP, ind_y, 5_p>, op_illegal,      up<DCP, ind_y, 8  >, up<NOP, zpg_x, 4>, op<CMP, zpg_x, 4>, op<DEC, zpg_x, 6>, up<DCP, zpg_x, 6>, op<CLD, imp, 2>, op<CMP, abs_y, 4_p>, up<NOP, imp, 2>, up<DCP, abs_y, 7>, up<NOP, abs_x, 4_p>, op<CMP, abs_x, 4_p>, op<DEC, abs_x, 7  >, up<DCP, abs_x, 7  >,
        /* 0xE- */ op<CPX, imm, 2>, op<SBC, x_ind, 6  >, up<NOP, imm, 2>, up<ISB, x_ind, 8  >, op<CPX, zpg,   3>, op<SBC, zpg,   3>, op<INC, zpg,   5>, up<ISB, zpg,   5>, op<INX, imp, 2>, op<SBC, imm,   2  >, op<NOP, imp, 2>, up<SBC, imm,   2>, op<CPX, abs,   4  >, op<SBC, abs,   4  >, op<INC, abs,   6  >, up<ISB, abs,   6  >,
        /* 0xF- */ op<BEQ, rel, 2>, op<SBC, ind_y, 5_p>, op_illegal,      up<ISB, ind_y, 8  >, up<NOP, zpg_x, 4>, op<SBC, zpg_x, 4>, op<INC, zpg_x, 6>, up<ISB, zpg_x, 6>, op<SED, imp, 2>, op<SBC, abs_y, 4_p>, up<NOP, imp, 2>, up<ISB, abs_y, 7>, up<NOP, abs_x, 4_p>, op<SBC, abs_x, 4_p>, op<INC, abs_x, 7  >, up<ISB, abs_x, 7  >
    };
}

template<class Bus>
template<instruction_tag InstTag, addressing_mode_tag AddrModeTag, num_cycles NumCycles, instruction_type InstType>
void opcode_decoder<Bus>::execute(vcpu<Bus>& cpu, std::uint8_t opcode) {
    absolute_address const init_pc = cpu.pc();
    //cpu.set_pc(cpu.pc() + 1);  // 1 = opcode size

    typename AddrModeTag::operand const op(init_pc + 1, cpu.system_bus());
    cpu.set_pc(init_pc + 1 + op.size);

    constexpr bool add_cycle_on_page_cross = NumCycles.increment_on_page_cross;
    operand_access<Bus, AddrModeTag, add_cycle_on_page_cross> op_access(cpu, op);

    cpu.template disasm_callback<InstTag, AddrModeTag, InstType>(opcode, init_pc, op, op_access);

    if constexpr (std::is_same_v<AddrModeTag, addressing_mode_tags::imp>)
        cpu.op(InstTag{});
    else
        cpu.op(InstTag{}, op_access);

    cpu.advance_cycle_counter(NumCycles.value);
}

template<class Bus>
void opcode_decoder<Bus>::op_illegal(vcpu<Bus>& cpu, std::uint8_t opcode) {
    throw illegal_opcode(cpu.pc(), opcode);
}

} // namespace emu::cpu
