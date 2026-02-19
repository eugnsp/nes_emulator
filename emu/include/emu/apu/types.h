#pragma once

#include "emu/utility/cyclic_counter.h"

#include <cstdint>
#include <variant>

namespace emu::apu {

enum class duty_mode : std::uint8_t {};

/** Length counter index (5-bit value, valid range: 0-31). */
enum class length_counter_index : std::uint8_t {};

enum class noise_period_index : std::uint8_t {};

/** Checks whether a length counter index value is within the valid range (for assertions). */
bool is_valid(length_counter_index) noexcept;

using four_step_sequencer = cyclic_counter<std::uint8_t, 4>;
using five_step_sequencer = cyclic_counter<std::uint8_t, 5>;

using frame_sequencer = std::variant<four_step_sequencer, five_step_sequencer>;

} // namespace emu::apu
