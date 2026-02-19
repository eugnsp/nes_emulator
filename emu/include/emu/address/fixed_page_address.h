#pragma once

#include "emu/address/arithmetic_address_mixin.h"
#include "emu/address/page.h"
#include "emu/address/raw_address.h"

#include <cstdint>
#include <utility>

namespace emu {

/**
 * 8-bit address with a page fixed at compile time.
 */
template<page_index Page>
class fixed_page_address : public detail::raw_address<std::uint8_t, std::int16_t>, public detail::arithmetic_address_mixin {
public:
    using raw_address::raw_address;

    /** Returns the page index of the address. */
    static constexpr page_index page() noexcept {
        return page_index{Page};
    }

    /** Converts the address to a 16-bit abstract address. */
    constexpr operator abstract_address() const noexcept {
        return abstract_address(Page, my_addr);
    }

private:
    friend arithmetic_address_mixin;

    constexpr void advance(difference_type n) noexcept {
        my_addr += n;
    }
};

/** Zero page 8-bit address. */
using zp_address = fixed_page_address<page_index{0}>;

/** Stack page 8-bit address. */
using sp_address = fixed_page_address<page_index{1}>;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<page_index Page1, page_index Page2>
constexpr bool operator==(fixed_page_address<Page1> addr1, fixed_page_address<Page2> addr2) noexcept {
    return std::pair(Page1, addr1.to_uint()) == std::pair(Page2, addr2.to_uint());
}

template<page_index Page1, page_index Page2>
constexpr auto operator<=>(fixed_page_address<Page1> addr1, fixed_page_address<Page2> addr2) noexcept {
    return std::pair(Page1, addr1.to_uint()) <=> std::pair(Page2, addr2.to_uint());
}

} // namespace emu
