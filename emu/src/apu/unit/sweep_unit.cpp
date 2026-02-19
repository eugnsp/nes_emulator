#include "emu/apu/unit/sweep_unit.h"

#include <cstdint>

namespace emu::apu {

template<sweep_arithmetic_mode Mode>
void sweep_unit::clock(channel_timer& timer) noexcept {
    if (my_sweep_reload) {
        if (my_sweep_enabled && my_sweep_value == 0)
            sweep<Mode>(timer);
        my_sweep_value = my_sweep_period + 1;
        my_sweep_reload = false;
    } else if (my_sweep_value > 0) {
        --my_sweep_value;
    } else {
        if (my_sweep_enabled)
            sweep<Mode>(timer);

        my_sweep_value = my_sweep_period + 1;
    }
}

template<sweep_arithmetic_mode Mode>
void sweep_unit::sweep(channel_timer& timer) noexcept {
    std::int16_t delta = timer.period() >> my_sweep_shift;
    if (my_sweep_negate)
        switch (Mode) {
            case sweep_arithmetic_mode::ones_complement:
                delta = ~delta; break;
            case sweep_arithmetic_mode::twos_complement:
                delta = -delta; break;
        }

    std::int16_t const new_period = static_cast<std::int16_t>(timer.period()) + delta;
    timer.set_period(clamp_to_zero(new_period));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template void sweep_unit::clock<sweep_arithmetic_mode::ones_complement>(channel_timer&);
template void sweep_unit::clock<sweep_arithmetic_mode::twos_complement>(channel_timer&);

} // namespace emu::apu
