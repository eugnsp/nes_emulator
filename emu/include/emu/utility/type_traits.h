#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace emu {

namespace detail {

template<std::size_t N, typename... Ints>
struct smallest_size_type_impl : std::type_identity<std::size_t> {};

template<std::size_t N, typename Int, typename... Ints>
struct smallest_size_type_impl<N, Int, Ints...> :
    std::conditional_t<(N <= std::numeric_limits<Int>::max()),
        std::type_identity<Int>,
        smallest_size_type_impl<N, Ints...>
    > {};

} // namespace detail

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Returns the smallest fixed-width unsigned integer type that can represent values in the range [0, N]. */
template<std::size_t N>
using smallest_size_type = detail::smallest_size_type_impl<N, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t>::type;

/** This concept is satisfied iff T denotes the same type as at least one type in Us. */
template<class T, class... Us>
concept same_as_any_of = (std::same_as<T, Us> || ...);

/** Generic type list. */
template<class...> struct type_list {};

} // namespace emu
