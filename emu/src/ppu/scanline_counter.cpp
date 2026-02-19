#include "emu/ppu/scanline_counter.h"

namespace emu::ppu {

bool scanline_counter::is_visible() const noexcept {
    return value() < visible_scanline_end;
}

bool scanline_counter::is_pre_render() const noexcept {
    return value() == pre_render_scanline;
}

bool scanline_counter::is_render() const noexcept {
    return is_visible() || is_pre_render();
}

} // namespace emu::ppu
