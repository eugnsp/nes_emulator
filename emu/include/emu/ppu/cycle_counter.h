#pragma once

#include "emu/utility/cyclic_counter.h"

#include <cstdint>

namespace emu::ppu {

static constexpr std::uint16_t num_cycles_per_scanline = 341;

/** A cyclic counter that tracks the current PPU cycle within a scanline. */
class cycle_counter : public cyclic_counter<std::uint16_t, num_cycles_per_scanline> {
public:
    bool is_visible() const noexcept;

    bool is_bg_prefetch() const noexcept;
    bool is_bg_fetch() const noexcept;

private:
    static constexpr value_type visible_cycle_begin = 1;
    static constexpr value_type visible_cycle_end   = 257;

    static constexpr value_type bg_prefetch_cycle_begin = 321;
    static constexpr value_type bg_prefetch_cycle_end   = 337;

    /*
    static constexpr std::uint16_t sprite_fetch_cycles_begin        = 257;
    static constexpr std::uint16_t sprite_fetch_cycles_end          = 321;  // [257, 321)
    static constexpr std::uint16_t idle_cycles_begin                = 337;
    static constexpr std::uint16_t idle_cycles_end                  = 341;  // [337, 341)
    static constexpr std::uint16_t cycles_per_scanline              = 341;
    */
};

} // namespace emu::ppu
