#pragma once

#include "emu/address/abstract_address.h"
#include "emu/address/arithmetic_address_mixin.h"
#include "emu/utility/bit_ops.h"

namespace emu {

/**
 * 16-bit address confined to a single memory page, with arithmetic wrapping within that page.
 */
class same_page_address : public abstract_address, public detail::arithmetic_address_mixin {
public:
    using abstract_address::abstract_address;

private:
    friend arithmetic_address_mixin;

    constexpr void advance(difference_type n) noexcept {
        my_addr = to_word(get_lo_byte(my_addr) + n, get_hi_byte(my_addr));
    }
};

} // namespace emu
