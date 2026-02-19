#include "emu/apu/unit/noise_lfsr.h"

#include "emu/utility/bit_ops.h"

#include <cstdint>
#include <utility>

namespace emu::apu {

/** Returns the least significant bit of the shift register. */
bool noise_lfsr::lsb() const noexcept {
    return is_lsb_set(my_state);
}

/** Sets the LFSR feedback mode. */
void noise_lfsr::set_mode(noise_lfsr_mode mode) noexcept {
    my_mode = mode;
}

/** Advances the shift register. */
void noise_lfsr::advance() noexcept {
    std::uint8_t const offset = std::to_underlying(my_mode);
    bool const feedback_bit = is_lsb_set(my_state) ^ is_lsb_set(my_state >> offset);

    static constexpr std::size_t msb_bit_pos = 14;
    my_state = (my_state >> 1) | (feedback_bit << msb_bit_pos);
}

} // namespace emu::apu
