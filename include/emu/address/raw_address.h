#pragma once

#include "emu/address/types.h"
#include "emu/types.h"
#include "emu/utility.h"

#include <compare>

namespace emu {

/** Address undelying integral type selector. */
template<address_size> struct address_underlying_type_selector;

template<> struct address_underlying_type_selector<address_size::byte> { using type = uint8_t; };
template<> struct address_underlying_type_selector<address_size::word> { using type = uint16_t; };

/**
 * A raw address: holds a value of an underlying unsigned 8-bit or 16-bit type.
 * Raw addresses can be compared but don't support address arithmetic.
 */
template<address_size Size>
class raw_address {
public:
    using underlying_type = address_underlying_type_selector<Size>::type;

public:
    raw_address() = default;

    explicit raw_address(underlying_type addr) noexcept
        : my_addr(addr) {}

    underlying_type value() const noexcept {
        return my_addr;
    }

    address_page_index page() const noexcept
    requires(Size == address_size::word) {
        return address_page_index{get_hi_byte(my_addr)};
    }

    uint8_t offset() const noexcept {
        return get_lo_byte(my_addr);
    }

    friend auto operator<=>(raw_address, raw_address) = default;

protected:
    underlying_type my_addr = 0;
};

} // namespace emu
