#pragma once

#include "emu/controller/vcontroller.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <unordered_map>

namespace emu::app {

/** Keyboard-based NES controller implementation. */
class keyboard_controller : public vcontroller {
public:
    keyboard_controller();

    /** Processes an SFML keyboard event and updates button states accordingly; safe to call concurrently. */
    void process_keyboard_event(sf::Event);

private:
    struct button_ex { buttons button; bool turbo_mode = false; };
    using buttons_map = std::unordered_map<sf::Keyboard::Key, button_ex>;

    buttons_map my_buttons_map;
};

} // namespace emu::app
