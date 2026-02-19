#include "main_window.h"

namespace emu::app {

main_window::main_window() {
    my_window.create(sf::VideoMode(window_width, window_height), "NES emulator");
    my_window.setVerticalSyncEnabled(true);

    my_texture.create(emu::ppu::frame_buffer::width, emu::ppu::frame_buffer::height);
    my_sprite.setTexture(my_texture, true);
    my_sprite.setScale(pixel_scale, pixel_scale);
}

void main_window::update_image(ppu::frame_buffer::image_span image) {
    my_texture.update(reinterpret_cast<sf::Uint8 const*>(image.data()));
}

void main_window::display() {
    my_window.draw(my_sprite);
    my_window.display();
}

void main_window::close() {
    my_window.close();
}

bool main_window::is_open() const {
    return my_window.isOpen();
}

std::optional<sf::Event> main_window::poll_event() {
    sf::Event event;
    if (my_window.pollEvent(event))
        return event;
    else
        return std::nullopt;
}

} // namespace emu::app
