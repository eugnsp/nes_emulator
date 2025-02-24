#pragma once

#include "emu/address/absolute_address.h"
#include "emu/bus/system_bus.h"
#include "emu/types.h"

namespace emu {

/** Null (zero-byte) operand. */
class null_operand {
public:
    static constexpr size_t size = 0;

public:
    /* Does nothing, to be used in generic context. */
    template<system_bus SystemBus>
    null_operand(absolute_address, SystemBus const&) {}
};

/** One-byte operand. */
class byte_operand {
public:
    static constexpr size_t size = 1;

public:
    /** Reads a one-byte operand in the constructor and caches it inside the object. */
    template<system_bus SystemBus>
    byte_operand(absolute_address addr, SystemBus const& bus)
        : my_operand(bus.load_byte(addr)) {}

    uint8_t operator*() const noexcept {
        return my_operand;
    }

private:
    uint8_t const my_operand;
};

/** Two-byte operand. */
class word_operand {
public:
    static constexpr size_t size = 2;

public:
    /** Reads a two-byte operand in the constructor and caches it inside the object. */
    template<system_bus SystemBus>
    word_operand(absolute_address addr, SystemBus const& bus)
        : my_operand(bus.load_word(addr)) {}

    uint16_t operator*() const noexcept {
        return my_operand;
    }

private:
    uint16_t const my_operand;
};

} // namespace emu
