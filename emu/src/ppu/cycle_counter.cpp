#include "emu/ppu/cycle_counter.h"

#include "emu/utility/functions.h"

namespace emu::ppu {

bool cycle_counter::is_visible() const noexcept {
    return is_between(visible_cycle_begin, value(), visible_cycle_end);
}

bool cycle_counter::is_bg_prefetch() const noexcept {
    return is_between(bg_prefetch_cycle_begin, value(), bg_prefetch_cycle_end);
}

bool cycle_counter::is_bg_fetch() const noexcept {
    return is_visible() || is_bg_prefetch();
}

} // namespace emu::ppu
