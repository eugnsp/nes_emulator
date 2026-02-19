#pragma once

#include "emu/ppu/frame_buffer.h"

#include <SFML/Graphics.hpp>

namespace emu::app {

class main_window {
public:
    main_window();

    void update_image(ppu::frame_buffer::image_span);

    void display();

    void close();

    bool is_open() const;

    std::optional<sf::Event> poll_event();

private:
    static constexpr int pixel_scale = 4;

    static constexpr unsigned int window_width  = pixel_scale * ppu::frame_buffer::width;
    static constexpr unsigned int window_height = pixel_scale * ppu::frame_buffer::height;

private:
    sf::RenderWindow my_window;
    sf::Texture my_texture;
    sf::Sprite  my_sprite;
};

} // namespace emu::app
