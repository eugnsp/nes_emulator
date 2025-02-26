#pragma once

#include "emu/cpu/operand.h"

#include <concepts>

namespace emu {

namespace detail { struct addressing_mode_tag {}; }

template<class AddressingModeTag>
concept addressing_mode_tag =
    std::derived_from<AddressingModeTag, detail::addressing_mode_tag> &&
    requires { typename AddressingModeTag::operand; };

/** Instruction addressing mode tags. */
namespace addressing_mode_tags {

/** Defines a tag for an addressing mode. */
#define EMU_DEFINE_ADDRESSING_MODE_TAG(AddressingMode, Operand)         \
    struct AddressingMode : detail::addressing_mode_tag {               \
        using operand = Operand;                                        \
    }

EMU_DEFINE_ADDRESSING_MODE_TAG(imm,   byte_operand);  // immediate
EMU_DEFINE_ADDRESSING_MODE_TAG(imp,   null_operand);  // implied
EMU_DEFINE_ADDRESSING_MODE_TAG(acc,   null_operand);  // accumulator (implied)

EMU_DEFINE_ADDRESSING_MODE_TAG(rel,   byte_operand);  // relative

EMU_DEFINE_ADDRESSING_MODE_TAG(abs,   word_operand);  // absolute
EMU_DEFINE_ADDRESSING_MODE_TAG(abs_x, word_operand);  // absolute, X-indexed
EMU_DEFINE_ADDRESSING_MODE_TAG(abs_y, word_operand);  // absolute, Y-indexed

EMU_DEFINE_ADDRESSING_MODE_TAG(ind,   word_operand);  // indirect
EMU_DEFINE_ADDRESSING_MODE_TAG(x_ind, byte_operand);  // X-indexed, indirect
EMU_DEFINE_ADDRESSING_MODE_TAG(ind_y, byte_operand);  // indirect, Y-indexed

EMU_DEFINE_ADDRESSING_MODE_TAG(zpg,   byte_operand);  // zeropage
EMU_DEFINE_ADDRESSING_MODE_TAG(zpg_x, byte_operand);  // zeropage, X-indexed
EMU_DEFINE_ADDRESSING_MODE_TAG(zpg_y, byte_operand);  // zeropage, Y-indexed

} // namespace addressing_modes

} // namespace emu
