#pragma once

#include <algorithm>
#include <concepts>
#include <type_traits>

namespace emu {

template<std::totally_ordered T>
constexpr bool is_between(T const& left, T const& value, T const& right) noexcept {
    return left <= value && value < right;
}

/** Returns a given value if it is positive, and zero otherwise. */
template<std::signed_integral T>
std::make_unsigned_t<T> clamp_to_zero(T value) {
	return std::max(T{0}, value);
}

} // namespace emu
