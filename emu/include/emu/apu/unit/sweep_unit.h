#pragma once

#include "emu/apu/unit/channel_timer.h"
#include "emu/utility/functions.h"

#include <cstdint>
#include <concepts>

namespace emu::apu {

/** Type of subtraction used by the pulse channel sweep unit. */
enum class sweep_arithmetic_mode {
    ones_complement,  // Subtraction with extra -1 (pulse channel 1)
    twos_complement   // Normal subtraction        (pulse channel 2)
};

class sweep_unit {
public:
    template<class SweepRegister>
    void configure(SweepRegister reg) noexcept {
	    my_sweep_enabled = reg.is_enabled();
    	my_sweep_period = reg.period();
    	my_sweep_negate = reg.is_negate();
    	my_sweep_shift = reg.shift_count();
    	my_sweep_reload = true;
	}

	template<sweep_arithmetic_mode>
    void clock(channel_timer&) noexcept;

private:
	template<sweep_arithmetic_mode>
    void sweep(channel_timer&) noexcept;

	template<sweep_arithmetic_mode>
	static std::int16_t subtract(std::uint16_t, std::uint16_t) noexcept;

private:
    bool my_sweep_enabled = false;
    bool my_sweep_reload = false;
	bool my_sweep_negate = false;
	std::uint8_t my_sweep_shift = 0;
	std::uint8_t my_sweep_period = 0;
	std::uint8_t my_sweep_value = 0;
};

} // namespace emu::apu
