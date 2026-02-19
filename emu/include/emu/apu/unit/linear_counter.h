#pragma once

#include <cstdint>

namespace emu::apu {

class linear_counter {
public:

    void schedule_reload() {
        my_counter_reload = true;
    }

    void set_period(std::uint8_t period) {
        my_counter_period = period;
    }

    void clock() {
        if (my_counter_reload)
		    my_curr_counter = my_counter_period;
	    else if (my_curr_counter > 0)
            --my_curr_counter;

	    if (!my_control)
		   my_counter_reload = false;
    }

    bool is_non_zero() const {
        return my_curr_counter > 0;
    }

    void set_control(bool state) {
        my_control = state;
    }

private:
    std::uint8_t my_counter_period = 0;
    std::uint8_t my_curr_counter = 0;
    bool my_counter_reload = false;
    bool my_control = false;
};

} // namespace emu::apu
