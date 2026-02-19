#pragma once

#include <concepts>
#include <cstdint>

namespace emu::apu::concepts {

/**
 * Envelope control register concept. Models the common interface of pulse wave and noise channel
 * control registers ($4000/$4004/$400C), which provide envelope configuration bits.
 */
template<class ControlRegister>
concept envelope_control_register =
    requires(ControlRegister const reg) {
        { reg.envelope_parameter()       } -> std::same_as<std::uint8_t>;
        { reg.has_constant_volume()      } -> std::same_as<bool>;
        { reg.is_length_counter_halted() } -> std::same_as<bool>;
    };

} // namespace emu::apu::concepts
