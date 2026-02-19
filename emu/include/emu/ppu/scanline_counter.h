#pragma once

#include "emu/utility/cyclic_counter.h"

#include <cstdint>

namespace emu::ppu {

constexpr std::uint16_t num_scanlines_per_frame = 262;

class scanline_counter : public cyclic_counter<std::uint16_t, num_scanlines_per_frame> {
public:
    bool is_visible() const noexcept;

    bool is_pre_render() const noexcept;
    bool is_render() const noexcept;

private:
    static constexpr value_type visible_scanline_end = 240;
    static constexpr value_type pre_render_scanline  = 261;
};

} // namespace emu::ppu
