#include "emu/cpu/exceptions.h"

#include "emu/address/absolute_address.h"

#include <cstdint>
#include <format>
#include <string>

namespace emu::cpu {

cpu_exception::cpu_exception(absolute_address pc, std::string const& message) :
    runtime_error(message), my_pc(pc) {}

absolute_address cpu_exception::address() const noexcept {
    return my_pc;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

illegal_opcode::illegal_opcode(absolute_address pc, std::uint8_t opcode) :
    cpu_exception(pc, std::format("Bad opcode ${:02X} at {:04X}", opcode, pc.to_uint())) {}

infinite_loop::infinite_loop(absolute_address pc) :
    cpu_exception(pc, std::format("Infinite loop at {:04X}", pc.to_uint())) {}

} // namespace emu::cpu
