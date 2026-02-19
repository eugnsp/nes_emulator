#pragma once

#include <concepts>

namespace emu::detail {

/**
 * A mixin class that provides standard arithmetic operations for address-like types.
 */
class arithmetic_address_mixin {
public:
    template<class Address>
    constexpr Address& operator+=(this Address& addr, Address::difference_type n) noexcept {
        addr.advance(n);
        return addr;
    }

    template<class Address>
    constexpr Address& operator-=(this Address& addr, Address::difference_type n) noexcept {
        addr.advance(-n);
        return addr;
    }

    template<class Address>
    constexpr Address& operator++(this Address& addr) noexcept {
        return (addr += 1);
    }

    template<class Address>
    constexpr Address operator++(this Address& addr, int) noexcept {
        Address const old = addr;
        ++addr;
        return old;
    }

    template<class Address>
    constexpr Address& operator--(this Address& addr) noexcept {
        return (addr -= 1);
    }

    template<class Address>
    constexpr Address operator--(this Address& addr, int) noexcept {
        Address const old = addr;
        --addr;
        return old;
    }

protected:
    ~arithmetic_address_mixin() = default;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<std::derived_from<arithmetic_address_mixin> Address>
constexpr Address operator+(Address addr, typename Address::difference_type n) noexcept {
    return (addr += n);
}

template<std::derived_from<arithmetic_address_mixin> Address>
constexpr Address operator+(typename Address::difference_type n, Address addr) noexcept {
    return addr + n;
}

template<std::derived_from<arithmetic_address_mixin> Address>
constexpr Address operator-(Address addr, typename Address::difference_type n) noexcept {
    return (addr -= n);
}

template<std::derived_from<arithmetic_address_mixin> Address>
constexpr Address::difference_type operator-(Address addr1, Address addr2) noexcept {
    return
        static_cast<Address::difference_type>(addr1.to_uint()) -
        static_cast<Address::difference_type>(addr2.to_uint());
}

} // namespace emu::detail
