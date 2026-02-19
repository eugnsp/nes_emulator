#pragma once

#include <concepts>

namespace emu::detail {

/**
 * A raw address. Holds a value of an underlying unsigned type, doesn't support address arithmetic.
 */
template<std::unsigned_integral UnderlyingType, std::signed_integral DifferenceType>
class raw_address {
public:
    using underlying_type = UnderlyingType;
    using difference_type = DifferenceType;

public:
    raw_address() = default;

    /** Constructs an address from an unsigned integer value. */
    explicit constexpr raw_address(underlying_type addr) noexcept : my_addr(addr) {}

    /** Returns the address value as an unsigned integer. */
    constexpr underlying_type to_uint() const noexcept {
        return my_addr;
    }

protected:
    underlying_type my_addr = 0;
};

} // namespace emu::detail
