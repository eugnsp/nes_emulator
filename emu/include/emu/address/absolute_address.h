#pragma once

#include "emu/address/abstract_address.h"
#include "emu/address/arithmetic_address_mixin.h"

namespace emu {

/**
 * 16-bit address with arithmetic wrapping within full 16-bit range.
 */
class absolute_address : public abstract_address, public detail::arithmetic_address_mixin {
public:
    using abstract_address::abstract_address;

    explicit constexpr absolute_address(abstract_address addr) : abstract_address(addr) {}

private:
    friend arithmetic_address_mixin;

    constexpr void advance(difference_type n) noexcept {
        my_addr += n;
    }
};

} // namespace emu
