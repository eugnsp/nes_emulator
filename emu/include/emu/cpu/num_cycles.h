#pragma once

#include <concepts>
#include <cstdint>

namespace emu::cpu {

namespace detail { struct page_cross_tag {}; }

struct num_cycles {
    std::uint8_t const value;
    bool const increment_on_page_cross;

    constexpr num_cycles(std::uint8_t n) : value(n), increment_on_page_cross(false) {}
    constexpr num_cycles(detail::page_cross_tag, std::uint8_t n) : value(n), increment_on_page_cross(true) {}
};

namespace page_cross_literals {

constexpr num_cycles operator""_p(unsigned long long n) {
    return num_cycles(detail::page_cross_tag(), static_cast<std::uint8_t>(n));
}

} // namespace page_cross_literals

} // namespace emu::cpu
