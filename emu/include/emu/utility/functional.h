#pragma once

namespace emu {

struct increment {
    template<class T>
    constexpr T operator()(T value) const noexcept {
        return ++value;
    }
};

struct decrement {
    template<class T>
    constexpr T operator()(T value) const noexcept {
        return --value;
    }
};

struct zero_constant_fn {
    template<class... Args>
    constexpr int operator()(Args const&...) const noexcept {
        return 0;
    }
};

inline constexpr zero_constant_fn zero_constant;

} // namespace emu
