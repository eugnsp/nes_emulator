#pragma once

#include <concepts>

namespace emu::concepts {

template<class Address>
concept address =
    std::regular<Address> &&
    requires {
        typename Address::underlying_type;
        typename Address::difference_type;
        requires std::unsigned_integral<typename Address::underlying_type>;
        requires std::signed_integral<  typename Address::difference_type>;
    } &&
    std::constructible_from<Address, typename Address::underlying_type> &&
    requires(Address addr) {
        { addr.to_uint() } -> std::same_as<typename Address::underlying_type>;
    };

template<class Address>
concept arithmetic_address =
    std::regular<Address> &&
    requires {
        typename Address::underlying_type;
        typename Address::difference_type;
        requires std::unsigned_integral<typename Address::underlying_type>;
        requires std::signed_integral<typename Address::difference_type>;
    } &&
    requires(Address addr, Address addr2, Address::difference_type n) {
        { addr += n } -> std::same_as<Address&>;
        { addr -= n } -> std::same_as<Address&>;
        { ++addr } -> std::same_as<Address&>;
        { addr++ } -> std::same_as<Address>;
        { --addr } -> std::same_as<Address&>;
        { addr-- } -> std::same_as<Address>;
        { addr + n } -> std::same_as<Address>;
        { n + addr } -> std::same_as<Address>;
        { addr - n } -> std::same_as<Address>;
        { addr - addr2 } -> std::same_as<typename Address::difference_type>;
    };

} // namespace emu::concepts
