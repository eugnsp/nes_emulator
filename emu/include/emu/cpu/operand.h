#pragma once

#include "emu/address/absolute_address.h"
#include "emu/cpu/fwd.h"

#include <cstdint>

namespace emu::cpu {

/** Null (zero-byte) operand. */
class null_operand {
public:
    static constexpr int size = 0;
    using value_type = void;

public:
    template<class Bus>
    null_operand(absolute_address, Bus const&) {}
};

/** One-byte operand. */
class byte_operand {
public:
    static constexpr int size = 1;
    using value_type = std::uint8_t;

public:
    template<class Bus>
    byte_operand(absolute_address pc, Bus& bus) : my_operand(bus.load(pc)) {}

    value_type operator*() const noexcept {
        return my_operand;
    }

private:
    value_type const my_operand;
};

/** Two-byte operand. */
class word_operand {
public:
    static constexpr int size = 2;
    using value_type = std::uint16_t;

public:
    template<class Bus>
    word_operand(absolute_address pc, Bus& bus) : my_operand(bus.load_word(pc)) {}

    value_type operator*() const noexcept {
        return my_operand;
    }

private:
    value_type const my_operand;
};

} // namespace emu::cpu
