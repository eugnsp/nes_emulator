#include "keyboard_controller.h"

#include <SFML/Window/Event.hpp>

#include <cassert>
#include <optional>

namespace emu::app {

keyboard_controller::keyboard_controller() {
    my_buttons_map[sf::Keyboard::Key::Left ] = {buttons::left };
    my_buttons_map[sf::Keyboard::Key::Right] = {buttons::right };
    my_buttons_map[sf::Keyboard::Key::Up   ] = {buttons::up    };
    my_buttons_map[sf::Keyboard::Key::Down ] = {buttons::down  };
    my_buttons_map[sf::Keyboard::Key::Space] = {buttons::select};
    my_buttons_map[sf::Keyboard::Key::Enter] = {buttons::start };
    my_buttons_map[sf::Keyboard::Key::A    ] = {buttons::a     };
    my_buttons_map[sf::Keyboard::Key::B    ] = {buttons::b     };

    // Turbo buttons
    my_buttons_map[sf::Keyboard::Key::S] = {buttons::a, true};
    my_buttons_map[sf::Keyboard::Key::N] = {buttons::b, true};
}

/** Processes an SFML keyboard event and updates button states accordingly. */
void keyboard_controller::process_keyboard_event(sf::Event event) {
    auto const pos = my_buttons_map.find(event.key.code);
    if (pos == my_buttons_map.end())
        return;

    if (event.type == sf::Event::KeyPressed)
        set_pressed(pos->second.button, pos->second.turbo_mode);
    else if (event.type == sf::Event::KeyReleased)
        set_released(pos->second.button);
    else
        assert(false && "Bad SFML event type");
}

} // namespace emu::app
