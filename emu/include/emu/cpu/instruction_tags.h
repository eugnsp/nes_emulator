#pragma once

#include <concepts>

namespace emu::cpu {

namespace detail { struct instruction_tag_base {}; }

/** CPU instruction tag type. */
template<class InstructionTag>
concept instruction_tag =
    std::derived_from<InstructionTag, detail::instruction_tag_base> &&
    std::same_as<decltype(InstructionTag::name), char const* const>;

/** Namespace for CPU instruction tags. */
namespace instruction_tags {

/** Defines a tag for a CPU instruction. */
#define EMU_DEFINE_INSTRUCTION_TAG(InstructionTag)                      \
    struct InstructionTag : detail::instruction_tag_base {              \
        static constexpr char const* name = #InstructionTag;            \
    }

/** Load/store operations. */
EMU_DEFINE_INSTRUCTION_TAG(LDA);  // load accumulator
EMU_DEFINE_INSTRUCTION_TAG(LDX);  // load X
EMU_DEFINE_INSTRUCTION_TAG(LDY);  // load Y
EMU_DEFINE_INSTRUCTION_TAG(LAX);  // load accumulator + load X (unofficial)
EMU_DEFINE_INSTRUCTION_TAG(STA);  // store accumulator
EMU_DEFINE_INSTRUCTION_TAG(STX);  // store X
EMU_DEFINE_INSTRUCTION_TAG(STY);  // store Y
EMU_DEFINE_INSTRUCTION_TAG(SAX);  // store accumulator AND X (unofficial)

/** Register transfers. */
EMU_DEFINE_INSTRUCTION_TAG(TAX);  // transfer accumulator to X
EMU_DEFINE_INSTRUCTION_TAG(TAY);  // transfer accumulator to Y
EMU_DEFINE_INSTRUCTION_TAG(TXA);  // transfer X to accumulator
EMU_DEFINE_INSTRUCTION_TAG(TYA);  // transfer Y to accumulator
EMU_DEFINE_INSTRUCTION_TAG(TSX);  // transfer stack pointer to X
EMU_DEFINE_INSTRUCTION_TAG(TXS);  // transfer X to stack pointer

/** Stack operations. */
EMU_DEFINE_INSTRUCTION_TAG(PHA);  // push accumulator on stack
EMU_DEFINE_INSTRUCTION_TAG(PHP);  // push processor status on stack
EMU_DEFINE_INSTRUCTION_TAG(PLA);  // pull accumulator from stack
EMU_DEFINE_INSTRUCTION_TAG(PLP);  // pull processor status from stack

/** Arithmetic. */
EMU_DEFINE_INSTRUCTION_TAG(ADC);  // add with carry
EMU_DEFINE_INSTRUCTION_TAG(SBC);  // subtract with carry
EMU_DEFINE_INSTRUCTION_TAG(ISB);  // increment + subtract with carry (unofficial)
EMU_DEFINE_INSTRUCTION_TAG(CMP);  // compare accumulator
EMU_DEFINE_INSTRUCTION_TAG(CPX);  // compare X
EMU_DEFINE_INSTRUCTION_TAG(CPY);  // compare Y
EMU_DEFINE_INSTRUCTION_TAG(DCP);  // decrement + compare accumulator (unofficial)

/** Increments and decrements. */
EMU_DEFINE_INSTRUCTION_TAG(INC);  // increment a memory location
EMU_DEFINE_INSTRUCTION_TAG(DEC);  // decrement a memory location
EMU_DEFINE_INSTRUCTION_TAG(INX);  // increment X
EMU_DEFINE_INSTRUCTION_TAG(DEX);  // decrement X
EMU_DEFINE_INSTRUCTION_TAG(INY);  // increment Y
EMU_DEFINE_INSTRUCTION_TAG(DEY);  // decrement Y

/** Logical.*/
EMU_DEFINE_INSTRUCTION_TAG(AND);  // logical AND
EMU_DEFINE_INSTRUCTION_TAG(EOR);  // exclusive OR
EMU_DEFINE_INSTRUCTION_TAG(ORA);  // inclusive OR
EMU_DEFINE_INSTRUCTION_TAG(BIT);  // bit test
EMU_DEFINE_INSTRUCTION_TAG(ANC);  // logical AND + set carry (unofficial)
EMU_DEFINE_INSTRUCTION_TAG(ALR);  // logical AND + set carry (unofficial)

/** Bit shifts. */
EMU_DEFINE_INSTRUCTION_TAG(ASL);  // arithmetic shift left
EMU_DEFINE_INSTRUCTION_TAG(LSR);  // logical shift right
EMU_DEFINE_INSTRUCTION_TAG(ROL);  // rotate left
EMU_DEFINE_INSTRUCTION_TAG(ROR);  // rotate right
EMU_DEFINE_INSTRUCTION_TAG(SLO);  // arithmetic shift left + inclusive OR (unofficial)
EMU_DEFINE_INSTRUCTION_TAG(SRE);  // logical shift right + exclusive OR (unofficial)
EMU_DEFINE_INSTRUCTION_TAG(RLA);  // rotate left + logical AND (unofficial)
EMU_DEFINE_INSTRUCTION_TAG(RRA);  // rotate right + add with carry (unofficial)

/** Jumps and calls. */
EMU_DEFINE_INSTRUCTION_TAG(JMP);  // jump to another location
EMU_DEFINE_INSTRUCTION_TAG(JSR);  // jump to a subroutine
EMU_DEFINE_INSTRUCTION_TAG(RTS);  // return from subroutine

/** Branches. */
EMU_DEFINE_INSTRUCTION_TAG(BCC);  // branch if carry flag clear
EMU_DEFINE_INSTRUCTION_TAG(BCS);  // branch if carry flag set
EMU_DEFINE_INSTRUCTION_TAG(BNE);  // branch if zero flag clear
EMU_DEFINE_INSTRUCTION_TAG(BEQ);  // branch if zero flag set
EMU_DEFINE_INSTRUCTION_TAG(BPL);  // branch if negative flag clear
EMU_DEFINE_INSTRUCTION_TAG(BMI);  // branch if negative flag set
EMU_DEFINE_INSTRUCTION_TAG(BVC);  // branch if overflow flag clear
EMU_DEFINE_INSTRUCTION_TAG(BVS);  // branch if overflow flag set

/** Status flag changes. */
EMU_DEFINE_INSTRUCTION_TAG(CLC);  // clear carry flag
EMU_DEFINE_INSTRUCTION_TAG(CLD);  // clear decimal flag
EMU_DEFINE_INSTRUCTION_TAG(CLI);  // clear interrupt disable flag
EMU_DEFINE_INSTRUCTION_TAG(CLV);  // clear overflow flag
EMU_DEFINE_INSTRUCTION_TAG(SEC);  // set carry flag
EMU_DEFINE_INSTRUCTION_TAG(SED);  // set decimal flag
EMU_DEFINE_INSTRUCTION_TAG(SEI);  // set interrupt disable flag

/** System functions. */
EMU_DEFINE_INSTRUCTION_TAG(BRK);  // force an interrupt
EMU_DEFINE_INSTRUCTION_TAG(RTI);  // return from interrupt
EMU_DEFINE_INSTRUCTION_TAG(NOP);  // no operation

} // namespace instruction_tags

} // namespace emu::cpu
