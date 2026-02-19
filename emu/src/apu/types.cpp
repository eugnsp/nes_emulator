#include "emu/apu/types.h"

#include <utility>

namespace emu::apu {

bool is_valid(length_counter_index idx) noexcept {
    return std::to_underlying(idx) < 32;
}

} // namespace emu::apu
