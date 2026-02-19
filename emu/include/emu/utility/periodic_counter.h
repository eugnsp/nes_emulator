#pragma once

#include <concepts>

namespace emu {

/**
 * A cyclic counter that decrements to 0, then wraps back to the period value.
 */
template<std::unsigned_integral T>
class periodic_counter {
public:
    using value_type = T;

public:
    /** Default-constructs the counter with zero value and zero period. */
    periodic_counter() = default;

    /**
     * Returns the current value of the counter. The value may temporarily
     * exceed the period if the period was reduced by calling set_period().
     */
    value_type value() const noexcept {
        return my_value;
    }

    /** Returns the current value of the counter's period. */
    value_type period() const noexcept {
        return my_period;
    }

    /**
     * Sets the period. The current counter value is not modified; if the period
     * is set to a value smaller than the current value, the counter may temporarily
     * hold a value greater than the period, which is allowed.
     */
    void set_period(value_type period) noexcept {
        my_period = period;
    }

    /** Resets the counter value to zero. */
    void reset() noexcept {
        my_value = 0;
    }

    /** Resets the counter to its period value. */
    void reload() noexcept {
        my_value = my_period;
    }

    /** Decrements the counter by one and returns the borrow bit. */
    bool decrement(bool wrap) noexcept {
        if (my_value > 0) [[likely]] {
            --my_value;
            return false;
        }

        if (wrap) {
            reload();
            return true;
        }
        else
            return false;
    }

private:
    value_type my_value = 0;
    value_type my_period = 0;
};

} // namespace emu
