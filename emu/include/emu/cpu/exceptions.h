#pragma once

#include "emu/address/absolute_address.h"

#include <cstdint>
#include <stdexcept>
#include <string>

namespace emu::cpu {

/** Base class for a CPU exception. */
class cpu_exception : public std::runtime_error {
protected:
    cpu_exception(absolute_address pc, std::string const& message);

public:
    absolute_address address() const noexcept;

private:
    absolute_address my_pc;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Illegal opcode exception. Thrown when opcode byte doesn't match any implemented instruction. */
class illegal_opcode : public cpu_exception {
public:
    explicit illegal_opcode(absolute_address pc, std::uint8_t opcode);
};

/** Infinite loop exception. Thrown when an infinite loop within a single instruction is detected. */
class infinite_loop : public cpu_exception {
public:
    explicit infinite_loop(absolute_address pc);
};

} // namespace emu::cpu
