#pragma once

#include <cstdint>

namespace emu::apu {

/**
 * Channel timer. Counts down and reloads when it reaches zero.
 */
class channel_timer {
public:
    std::uint16_t period() const {
        return my_period;
    }

    std::uint16_t value() const {
        return my_counter;
    }

    void set_period(std::uint16_t period) {
        my_period = period;
    }

    void set_period_lo(std::uint8_t period_lo_byte) noexcept {
        my_period = (my_period & 0xFF00) | period_lo_byte;
    }

    void set_period_hi(std::uint8_t period_hi_byte) noexcept {
        my_period = (my_period & 0x00FF) | (period_hi_byte << 8);
    }

    bool clock() {
        if (my_counter == 0) {
            my_counter = my_period;
            return true;
        }
        else {
            --my_counter;
            return false;
        }
    }

private:
    std::uint16_t my_period = 0;
    std::uint16_t my_counter = 0;
};

} // namespace emu::apu
