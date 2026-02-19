#pragma once

#include "emu/address/concepts.h"

namespace emu {

/**
 * Address bit mask.
 */
template<concepts::address Address>
class address_mask {
public:
    using underlying_type = Address::underlying_type;

public:
    /** Constructs an address mask from an unsigned integer value. */
    constexpr explicit address_mask(underlying_type mask) noexcept : my_mask(mask) {}

    /** Returns the address mask value as an unsigned integer. */
    constexpr underlying_type to_uint() const noexcept {
        return my_mask;
    }

    friend constexpr bool operator==(address_mask, address_mask) = default;

private:
    underlying_type my_mask = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Bitwise operators. */

template<class Address>
address_mask<Address> operator~(address_mask<Address> mask) noexcept {
    return address_mask<Address>(
        static_cast<Address::underlying_type>(~mask.to_uint())
    );
}

#define EMU_DEFINE_ADDRESS_MASK_BITWISE_BINARY_OPS(Op)                                              \
template<class Address>                                                                             \
constexpr Address operator Op(Address addr, address_mask<Address> mask) noexcept {                  \
    return Address(                                                                                 \
        static_cast<Address::underlying_type>(addr.to_uint() Op mask.to_uint())                     \
    );                                                                                              \
}                                                                                                   \
                                                                                                    \
template<class Address>                                                                             \
constexpr Address operator Op(address_mask<Address> mask, Address addr) noexcept {                  \
    return (addr Op mask);                                                                          \
}                                                                                                   \
                                                                                                    \
template<class Address>                                                                             \
constexpr Address& operator Op##=(Address& addr, address_mask<Address> mask) noexcept {             \
    addr = (addr Op mask);                                                                          \
    return addr;                                                                                    \
}

EMU_DEFINE_ADDRESS_MASK_BITWISE_BINARY_OPS(&)
EMU_DEFINE_ADDRESS_MASK_BITWISE_BINARY_OPS(|)
EMU_DEFINE_ADDRESS_MASK_BITWISE_BINARY_OPS(^)

#define EMU_DEFINE_ADDRESS_MASK_BITWISE_SHIFT_OPS(Op)                                               \
template<class Address>                                                                             \
constexpr address_mask<Address> operator Op(address_mask<Address> mask, int pos) noexcept {         \
    return address_mask<Address>(                                                                   \
        static_cast<Address::underlying_type>(mask.to_uint() Op pos)                                \
    );                                                                                              \
}                                                                                                   \
                                                                                                    \
template<class Address>                                                                             \
constexpr address_mask<Address>& operator Op##=(address_mask<Address>& mask, int pos) noexcept {    \
    mask = (mask Op pos);                                                                           \
    return mask;                                                                                    \
}

EMU_DEFINE_ADDRESS_MASK_BITWISE_SHIFT_OPS(<<)
EMU_DEFINE_ADDRESS_MASK_BITWISE_SHIFT_OPS(>>)

} // namespace emu
