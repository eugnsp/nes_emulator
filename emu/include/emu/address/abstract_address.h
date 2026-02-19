#pragma once

#include "emu/address/raw_address.h"
#include "emu/address/page.h"
#include "emu/utility/bit_ops.h"

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <utility>

namespace emu {

/**
 * 16-bit abstract address. Doesn't support address arithmetic.
 */
class abstract_address : public detail::raw_address<std::uint16_t, std::int32_t> {
public:
    using raw_address::raw_address;

    /** Constructs an address from b page index and an offset within that page. */
    explicit constexpr abstract_address(page_index page_idx, std::uint8_t offset = 0) noexcept :
        raw_address(to_word(offset, std::to_underlying(page_idx))) {}

    /** Returns the page index of the address. */
    constexpr page_index page() const noexcept {
        return page_index{get_hi_byte(my_addr)};
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline constexpr bool operator==(abstract_address addr1, abstract_address addr2) noexcept {
    return addr1.to_uint() == addr2.to_uint();
}

inline constexpr auto operator<=>(abstract_address addr1, abstract_address addr2) noexcept {
    return addr1.to_uint() <=> addr2.to_uint();
}

} // namespace emu

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<std::derived_from<emu::abstract_address> Address>
struct std::hash<Address> {
    std::size_t operator()(emu::abstract_address addr) const noexcept {
        constexpr std::hash<typename Address::underlying_type> addr_hash;
        return addr_hash(addr.to_uint());
    }
};
