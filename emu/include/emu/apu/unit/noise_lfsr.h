#pragma once

#include "emu/apu/types.h"

#include <cstdint>

namespace emu::apu {

/** Specifies which feedback tap bit index is used by the LFSR. */
enum class noise_lfsr_mode : std::uint8_t {
    normal    = 1,  // Bit 1 feedback (long sequence)
    shortened = 6   // Bit 6 feedback (short sequence)
};

/**
 * Linear-feedback shift 15-bit register (LFSR) used by the noise channel.
 */
class noise_lfsr {
public:
    /** Returns the least significant bit of the shift register. */
    bool lsb() const noexcept;

    /** Sets the LFSR feedback mode. */
    void set_mode(noise_lfsr_mode) noexcept;

    /** Advances the shift register. */
    void advance() noexcept;

private:
    std::uint16_t   my_state = 1;
    noise_lfsr_mode my_mode  = noise_lfsr_mode::normal;
};

} // namespace emu::apu
