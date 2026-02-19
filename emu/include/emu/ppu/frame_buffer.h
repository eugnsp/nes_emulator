#pragma once

#include "emu/utility/dynamic_array.h"
#include "emu/ppu/types.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <span>

namespace emu::ppu {

namespace detail {

/** 32‑bit RGBA color. */
struct color_rgba { std::uint8_t r, g, b, a = 255; };

} // namespace detail

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class frame_buffer_locked_view;

/**
 * Holds a single frame of pixels using double buffering. Pixels are written
 * sequentially, and the finished frame can be uploaded to an SFML texture.
 */
class frame_buffer {
public:
    static constexpr std::uint16_t width  = 256;
    static constexpr std::uint16_t height = 240;
    static constexpr std::size_t num_pixels = width * height;

public:
    frame_buffer();

    /** Writes a pixel and increments the internal write pointer. Must be called (width * height) times per frame. */
    void set_next(system_color_index, color_effects_flags);

    /** Marks the back buffer as ready and notifies a waiting thread. */
    void release();

    /** Waits for a completed frame and returns a locked view to the front image buffer. */
    frame_buffer_locked_view acquire();

private:
    friend frame_buffer_locked_view;
    using image_buffer = dynamic_array<detail::color_rgba, num_pixels>;

private:
    image_buffer::iterator my_curr_pixel;
    image_buffer my_front_buffer;
    image_buffer my_back_buffer;

    std::mutex my_mutex;
    std::condition_variable my_cv;
    bool my_image_ready = false;
};

/** RAII-type object that provides a locked view to the front image buffer. */
class frame_buffer_locked_view {
public:
    using image_span = std::span<detail::color_rgba const, frame_buffer::num_pixels>;

public:
    /** Releases the lock of the front image buffer. */
    ~frame_buffer_locked_view();

    /** Returns a span to the front image buffer's pixel data. */
    image_span span() const noexcept;

private:
    friend frame_buffer;
    frame_buffer_locked_view(frame_buffer&);

private:
    frame_buffer& my_frame_buffer;
    std::unique_lock<std::mutex> my_lock;
};

} // namespace emu::ppu
