#pragma once

#include "operand.h"
#include "emu/cpu/addressing_mode_tags.h"
#include "emu/utility/bit_ops.h"

#include <cstdint>
#include <format>
#include <string>

namespace emu::cpu::detail {

///////////////////////////////////////////////////////////////////////////////

template<addressing_mode_tag AddrMode, class Operand = AddrMode::operand>
std::string to_bytes_string(std::uint8_t opcode, Operand const& op) {
    if constexpr (Operand::size == 0)
        return std::format("{:02X}", opcode);
    else if constexpr (Operand::size == 1)
        return std::format("{:02X} {:02X}", opcode, *op);
    else if constexpr (Operand::size == 2)
        return std::format("{:02X} {:02X} {:02X}", opcode, get_lo_byte(*op), get_hi_byte(*op));
    else
        static_assert(false, "Unexpected operand size");
}

template<class OperandAccess>
inline std::string to_asm_string(addressing_mode_tags::imm, absolute_address /* pc */, byte_operand op, OperandAccess const&) {
    return std::format("#${:02X}", *op);
}

template<class OperandAccess>
inline std::string to_asm_string(addressing_mode_tags::imp, absolute_address /* pc */, null_operand, OperandAccess const&) {
    return {};
}

template<class OperandAccess>
inline std::string to_asm_string(addressing_mode_tags::acc, absolute_address /* pc */, null_operand, OperandAccess const&) {
    return "A";
}

template<class OperandAccess>
inline std::string to_asm_string(addressing_mode_tags::rel, absolute_address pc, byte_operand op, OperandAccess const&) {
    std::uint16_t const addr = pc.to_uint() + static_cast<std::int8_t>(*op + 2);
    return std::format("${:04X}", addr);
}

template<class OperandAccess>
inline std::string to_asm_string(addressing_mode_tags::abs, absolute_address /* pc */, word_operand op, OperandAccess const&) {
    return std::format("${:04X}", *op);
}

template<class OperandAccess>
inline std::string to_asm_string(addressing_mode_tags::abs_x, absolute_address /* pc */, word_operand op, OperandAccess const& op_access) {
    return std::format("${:04X},X @ {:04X} = {:02X}", *op, op_access.get().to_uint(), op_access.load());
}

template<class OperandAccess>
inline std::string to_asm_string(addressing_mode_tags::abs_y, absolute_address /* pc */, word_operand op, OperandAccess const& op_access) {
    return std::format("${:04X},Y @ {:04X} = {:02X}", *op, op_access.get().to_uint(), op_access.load());
}

template<class OperandAccess>
inline std::string to_asm_string(addressing_mode_tags::x_ind, absolute_address /* pc */, byte_operand op, OperandAccess const& op_access) {
    return std::format("(${:02X},X) @ {:04X} = {:02X}", *op, op_access.get().to_uint(), op_access.load());
}

template<class OperandAccess>
inline std::string to_asm_string(addressing_mode_tags::ind_y, absolute_address /* pc */, byte_operand op, OperandAccess const& op_access) {
    return std::format("(${:02X}),Y @ {:04X} = {:02X}", *op, op_access.get().to_uint(), op_access.load());
}

template<class OperandAccess>
inline std::string to_asm_string(addressing_mode_tags::zpg, absolute_address /* pc */, byte_operand op, OperandAccess const& op_access) {
    return std::format("${:02X} = {:02X}", *op, op_access.load());
}

template<class OperandAccess>
inline std::string to_asm_string(addressing_mode_tags::zpg_x, absolute_address /* pc */, byte_operand op, OperandAccess const& op_access) {
    return std::format("${:02X},X @ {:02X} = {:02X}", *op, op_access.get().to_uint(), op_access.load());
}

template<class OperandAccess>
inline std::string to_asm_string(addressing_mode_tags::zpg_y, absolute_address /* pc */, byte_operand op, OperandAccess const& op_access) {
    return std::format("${:02X},Y @ {:02X} = {:02X}", *op, op_access.get().to_uint(), op_access.load());
}

template<class OperandAccess>
inline std::string to_asm_string(addressing_mode_tags::ind, absolute_address /* pc */, word_operand op, OperandAccess const& op_access) {
    return std::format("(${:04X}) = {:04X}", *op, op_access.get().to_uint());
}

} // namespace emu::cpu::detail
