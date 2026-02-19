#include "emu/controller/vcontroller.h"

#include "emu/utility/bit_ops.h"

#include <cstdint>

namespace emu {

/** Reads one bit from the controller shift register. */
std::uint8_t vcontroller::load() noexcept {
    constexpr std::uint8_t released{0x00}, pressed{0x01};

    if (my_button_index > 7)
        return pressed;

    bool is_pressed = my_buttons_state.is_nth_set(my_button_index);
    if (is_pressed && my_turbo_state.is_nth_set(my_button_index))
        is_pressed = my_turbo_latch;

    if (!my_shift_reg_strobe)
        ++my_button_index;

    return is_pressed ? pressed : released;
}

void vcontroller::store(std::uint8_t value) noexcept {
    my_shift_reg_strobe = is_lsb_set(value);
    if (my_shift_reg_strobe)
        my_button_index = 0;
}

/** Mark button as pressed and optionally set turbo mode for it. */
void vcontroller::set_pressed(buttons button, bool turbo_mode) noexcept {
    my_buttons_state.set(button, true);
    my_turbo_state.set(button, turbo_mode);
}

/** Mark button as released. */
void vcontroller::set_released(buttons button) noexcept {
    my_buttons_state.set(button, false);
}

void vcontroller::clock_turbo() noexcept {
    my_turbo_latch = !my_turbo_latch;
}

} // namespace emu
