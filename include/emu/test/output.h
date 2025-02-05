#pragma once

#include <algorithm>
#include <cstddef>
#include <format>
#include <iterator>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace test {

/** Converts a color name to its ANSI escape sequence color code. */
inline constexpr int name_to_color_code(std::string_view clr_name) {
    struct color { int code; std::string_view name; };

    constexpr color colors[] = {
        {30, "black"  }, {31, "red"    },
        {32, "green"  }, {33, "yellow" },
        {34, "blue"   }, {35, "magenta"},
        {36, "cyan"   }, {37, "white"  }
    };

    auto const pos = std::ranges::find(colors, clr_name, &color::name);
    if (pos != std::ranges::end(colors))
        return pos->code;
    else
        throw std::format_error("bad color name");
}

template<class Storage>
struct colorize_wrapper {
    std::string_view fmt;
    Storage args_storage;
};

/**
 * Creates a helper wrapper object for colorized output. It has
 * reference semantics and should not outlive the arguments.
 *
 * Usage example: std::format("{:green}", colorize("{}", 10));
 */
template<class... Args>
auto colorize(std::format_string<Args...> fmt, Args&&... args) {
    return colorize_wrapper{fmt.get(), std::make_format_args(args...)};
}

template<class T>
std::string to_bytes_string(T const& value) {
    std::string str(1 + 5 * sizeof(T), ' ');

    auto it = str.begin();
    *it = '{';

    for (auto const ch : std::as_bytes(std::span(&value, 1)))
        it = std::format_to(++it, "0x{:02X}", std::to_integer<int>(ch));

    *it = '}';
    return str;
}

template<class T>
std::string to_string(T const& value) {
    if constexpr (std::formattable<T, char>)
        return std::format("{}", value);
    else if constexpr (std::is_enum_v<T>)
        return std::format("{}", std::to_underlying(value));
    else
        return to_bytes_string(value);
}

} // namespace test

template<class Storage>
struct std::formatter<test::colorize_wrapper<Storage>> {
public:
    constexpr auto parse(std::format_parse_context& ctx) {
        auto const last = std::find(ctx.begin(), ctx.end(), '}');
        my_color_code = test::name_to_color_code(std::string_view(ctx.begin(), last));

        return last;
    }

    auto format(test::colorize_wrapper<Storage> const& clr, std::format_context& ctx) const {
        auto out = std::format_to(ctx.out(), "\033[{}m", my_color_code);
        out = std::vformat_to(out, clr.fmt, clr.args_storage);
        return std::format_to(out, "\033[0m");
    }

private:
    int my_color_code;
};
