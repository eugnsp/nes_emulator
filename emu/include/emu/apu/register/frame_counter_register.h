#pragma once

#include "emu/apu/types.h"
#include "emu/utility/bit_flags.h"

#include <cstdint>

namespace emu::apu {

/**
 * APU frame counter register ($4017, write only).
 */
class frame_counter_register : private bit_flags<std::uint8_t> {
public:
    using bit_flags::bit_flags;

    frame_sequencer make_frame_sequencer() const noexcept;

private:
    enum class flags : underlying_type {
        counter_mode = 0b1000'0000,  // Mode: 0 = 4-step, 1 = 5-step
        irq_inhibit  = 0b0100'0000   // IRQ inhibit flag
    };
};

} // namespace emu::apu
