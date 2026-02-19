#pragma once

#include "emu/utility/bit_ops.h"

#include <cstdint>

namespace emu::detail {

/**
 * A helper mixin class that provides 16-bit word read and write operations.
 */
class bus_word_access_mixin {
public:
    /** Reads a 16-bit word from memory or a device register. */
    template<class Self, class Address>
    std::uint16_t load_word(this Self& self, Address addr) {
        std::uint8_t const lo_byte = self.load(addr    );
        std::uint8_t const hi_byte = self.load(addr + 1);
        return to_word(lo_byte, hi_byte);
    }

    /** Writes a 16-bit word to memory or a device register. */
    template<class Self, class Address>
    void store_word(this Self const& self, Address addr, std::uint16_t value) {
        std::uint8_t const lo_byte = get_lo_byte(value);
        self.store(addr,     lo_byte);

        std::uint8_t const hi_byte = get_hi_byte(value);
        self.store(addr + 1, hi_byte);
    }

protected:
    ~bus_word_access_mixin() = default;
};

} // namespace emu::detail
