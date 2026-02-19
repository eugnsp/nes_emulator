#pragma once

#include <concepts>

namespace emu {

/**
 * A cyclic counter that advances from 0 to Modulus-1, then wraps back to 0.
 */
template<std::unsigned_integral T, T Modulus>
class cyclic_counter {
    static_assert(Modulus > 0, "Modulus must be positive");

public:
    using value_type = T;

public:
    /** Default-constructs the counter, setting its value to zero. */
    cyclic_counter() = default;

    /** Returns the current value of the counter in the range [0, Modulus). */
    value_type value() const noexcept {
        return my_value;
    }

    /** Returns the counter modulus. */
    static constexpr value_type modulus() noexcept {
        return Modulus;
    }

    /** Resets the counter value to zero. */
    void reset() noexcept {
        my_value = 0;
    }

    /** Increments the counter by one and returns the carry bit. */
    bool increment() noexcept {
        if (++my_value < Modulus) [[likely]]
            return false;

        reset();
        return true;
    }

private:
    value_type my_value = 0;
};

} // namespace emu
