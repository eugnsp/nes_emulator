#include "emu/ppu/frame_buffer.h"

#include "emu/utility/bit_ops.h"
#include "emu/utility/dynamic_array.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <ranges>

namespace emu::ppu {

constexpr detail::color_rgba system_palette[system_palette_size] = {
    /* 00 */ {0x54, 0x54, 0x54}, {0x00, 0x1E, 0x74}, {0x08, 0x10, 0x90}, {0x30, 0x00, 0x88},
    /* 04 */ {0x44, 0x00, 0x64}, {0x5C, 0x00, 0x30}, {0x54, 0x04, 0x00}, {0x3C, 0x18, 0x00},
    /* 08 */ {0x20, 0x2A, 0x00}, {0x08, 0x3A, 0x00}, {0x00, 0x40, 0x00}, {0x00, 0x3C, 0x00},
    /* 0C */ {0x00, 0x32, 0x3C}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
    /* 10 */ {0x98, 0x96, 0x98}, {0x08, 0x4C, 0xC4}, {0x30, 0x32, 0xEC}, {0x5C, 0x1E, 0xE4},
    /* 14 */ {0x88, 0x14, 0xB0}, {0xA0, 0x14, 0x64}, {0x98, 0x22, 0x20}, {0x78, 0x3C, 0x00},
    /* 18 */ {0x54, 0x5A, 0x00}, {0x28, 0x72, 0x00}, {0x08, 0x7C, 0x00}, {0x00, 0x76, 0x28},
    /* 1C */ {0x00, 0x74, 0x88}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
    /* 20 */ {0xEC, 0xEE, 0xEC}, {0x4C, 0x9A, 0xEC}, {0x78, 0x7C, 0xEC}, {0xB0, 0x62, 0xEC},
    /* 24 */ {0xE4, 0x54, 0xEC}, {0xEC, 0x58, 0xB4}, {0xEC, 0x6A, 0x64}, {0xD4, 0x88, 0x20},
    /* 28 */ {0xA0, 0xAA, 0x00}, {0x74, 0xC4, 0x00}, {0x4C, 0xD0, 0x20}, {0x38, 0xCC, 0x6C},
    /* 2C */ {0x38, 0xC2, 0xCC}, {0x3C, 0x3C, 0x3C}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
    /* 30 */ {0xEC, 0xEE, 0xEC}, {0xA8, 0xCC, 0xEC}, {0xBC, 0xBC, 0xEC}, {0xD4, 0xB2, 0xEC},
    /* 34 */ {0xEC, 0xAE, 0xEC}, {0xEC, 0xAE, 0xD4}, {0xEC, 0xBA, 0xA8}, {0xE4, 0xC4, 0x90},
    /* 38 */ {0xCC, 0xD2, 0x78}, {0xB4, 0xDE, 0x78}, {0xA8, 0xE2, 0x90}, {0x98, 0xE2, 0xB4},
    /* 3C */ {0xA0, 0xE2, 0xE2}, {0xA0, 0xA2, 0xA0}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

frame_buffer::frame_buffer() {
    my_curr_pixel = my_back_buffer.begin();
}

/** Writes a pixel and increments the internal write pointer. Must be called (width * height) times per frame. */
void frame_buffer::set_next(system_color_index color_idx, color_effects_flags color_eff) {
    assert(is_valid(color_idx));
    if (color_eff.grayscale)
        color_idx = bitwise_and(color_idx, system_color_index::grayscale_mask);

    detail::color_rgba color = system_palette[std::to_underlying(color_idx)];
    if (color_eff.emphasize_red)   { color.g /= 2; color.b /= 2; }
    if (color_eff.emphasize_green) { color.b /= 2; color.r /= 2; }
    if (color_eff.emphasize_blue)  { color.r /= 2; color.g /= 2; }

    assert(my_curr_pixel < my_back_buffer.end());
    *my_curr_pixel++ = color;
}

/** Marks the back buffer as ready and notifies a waiting thread. */
void frame_buffer::release() {
    assert(my_curr_pixel == my_back_buffer.end());

    {
        std::scoped_lock lock(my_mutex);

        std::swap(my_back_buffer, my_front_buffer);
        my_image_ready = true;
    }

    my_cv.notify_one();
    my_curr_pixel = my_back_buffer.begin();
}

/** Waits for a completed frame and returns a locked view to the image buffer. */
frame_buffer_locked_view frame_buffer::acquire() {
    return frame_buffer_locked_view(*this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Releases the lock of the front image buffer. */
frame_buffer_locked_view::~frame_buffer_locked_view() {
    my_frame_buffer.my_image_ready = false;
}

/** Returns a span to the front image buffer's pixel data. */
auto frame_buffer_locked_view::span() const noexcept -> image_span {
    return image_span(my_frame_buffer.my_front_buffer.data(), image_span::extent);
}

frame_buffer_locked_view::frame_buffer_locked_view(frame_buffer& buff) : my_frame_buffer(buff), my_lock(buff.my_mutex) {
    my_frame_buffer.my_cv.wait(my_lock, [this]{ return my_frame_buffer.my_image_ready; });
}

} // namespace emu::ppu
