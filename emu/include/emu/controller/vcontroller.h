#pragma once

#include "emu/controller/buttons.h"
#include "emu/utility/bit_flags.h"

#include <atomic>
#include <cstddef>
#include <cstdint>

namespace emu {

/** NES controller base class. Button state setters allow updating button states from another thread. */
class vcontroller {
public:
    /** Reads data from the controller shift register. */
    std::uint8_t load() noexcept;

    /** Writes data to the controller strobe register. */
    void store(std::uint8_t) noexcept;

    /** Toggles the turbo mode latch. */
    void clock_turbo() noexcept;

protected:
    vcontroller() = default;

    /** Mark button as pressed and optionally set turbo mode for it. */
    void set_pressed(buttons, bool turbo_mode) noexcept;

    /** Mark button as released. */
    void set_released(buttons) noexcept;

private:
    using buttons_state = bit_flags<std::uint8_t, std::atomic<std::uint8_t>>;

    buttons_state my_buttons_state;  // Pressed/released state of buttons
    buttons_state my_turbo_state;    // Per-button turbo mode enable flags

    bool my_turbo_latch = false;
    bool my_shift_reg_strobe = false;
    std::uint8_t my_button_index = 0;
};

} // namespace emu
