#include "emu/apu/vapu.h"

#include "emu/apu/register/frame_counter_register.h"
#include "emu/apu/register/status_register.h"
#include "emu/controller/vcontroller.h"

#include <array>
#include <cassert>
#include <cstddef>

namespace emu::apu {

/** Generates the pulse channel mixer lookup table. */
constexpr auto make_pulse_mixer_table() noexcept {
    std::array<float, 31> values;
    for (std::size_t i = 0; i < values.size(); ++i)
        values[i] = 95.52 * i / (8128.0 + 100.0 * i);

    return values;
}

/** Generates the TND (triangle, noise, DMC) mixer lookup table. */
constexpr auto make_tnd_mixer_table() noexcept {
    std::array<float, 203> values;
    for (std::size_t i = 0; i < values.size(); ++i)
        values[i] = 163.67 * i / (24329.0 + 100.0 * i);

    return values;
}

constexpr auto pulse_mixer_table = make_pulse_mixer_table();  // Pulse channel mixer lookup table
constexpr auto tnd_mixer_table   = make_tnd_mixer_table();    // TND (triangle, noise, DMC) mixer lookup table

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vapu::vapu(vcontroller& controller) : my_controller(controller) {}

/** Returns the current output of the APU in the range [0, 1]. */
float vapu::output() {
    std::uint8_t const output_pls = my_pulse1_channel.output() + my_pulse2_channel.output();
    assert(output_pls < pulse_mixer_table.size() && "Pulse channels output is out-of-bounds");

    std::uint8_t const output_tnd = 3 * my_triangle_channel.output() + 2 * my_noise_channel.output();
    assert(output_tnd < tnd_mixer_table.size() && "TND channels output is out-of-bounds");

    return pulse_mixer_table[output_pls] + tnd_mixer_table[output_tnd];
}

/** Reads a byte from an APU register. */
std::uint8_t vapu::load(abstract_address addr) noexcept {
    if (addr == apu_ctrl1_register)
        return my_controller.load();
    else if (addr == apu_status_register)
        return load_status();
    else
        return std::uint8_t{0x00};  // Not mapped (TODO: open bus)
}

/** Writes a byte to an APU register. */
void vapu::store(abstract_address addr, std::uint8_t value) noexcept {
    if (addr == apu_p1_ctrl_register)
        my_pulse1_channel.store_control(value);
    else if (addr == apu_p1_sweep_register)
        my_pulse1_channel.store_sweep(value);
    else if (addr == apu_p1_timer_lo_register)
        my_pulse1_channel.store_timer_lo(value);
    else if (addr == apu_p1_timer_hi_register)
        my_pulse1_channel.store_timer_hi(value);
    else if (addr == apu_p2_ctrl_register)
        my_pulse2_channel.store_control(value);
    else if (addr == apu_p2_sweep_register)
        my_pulse2_channel.store_sweep(value);
    else if (addr == apu_p2_timer_lo_register)
        my_pulse2_channel.store_timer_lo(value);
    else if (addr == apu_p2_timer_hi_register)
        my_pulse2_channel.store_timer_hi(value);
    else if (addr == apu_tri_control_register)
        my_triangle_channel.store_control(value);
    else if (addr == apu_tri_timer_lo_register)
        my_triangle_channel.store_timer_lo(value);
    else if (addr == apu_tri_timer_hi_register)
        my_triangle_channel.store_timer_hi(value);
    else if (addr == apu_noise_ctrl_register)
        my_noise_channel.store_control(value);
    else if (addr == apu_noise_mode_register)
        my_noise_channel.store_period(value);
    else if (addr == apu_noise_timer_register)
        my_noise_channel.store_timer(value);
    else if (addr == apu_status_register)
        store_status(value);
    else if (addr == apu_ctrl1_register)
        my_controller.store(value);
    else if (addr == apu_fc_register)
        store_frame_counter(value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Reads data from the status register. */
std::uint8_t vapu::load_status() const noexcept {
    status_register reg;

    reg.set_pulse1_channel_active(my_pulse1_channel.is_active());
    reg.set_pulse2_channel_active(my_pulse2_channel.is_active());
    reg.set_triangle_channel_active(my_triangle_channel.is_active());
    reg.set_noise_channel_active(my_noise_channel.is_active());

    return reg.to_uint();
}

/** Writes data into the status register. */
void vapu::store_status(std::uint8_t value) noexcept {
    status_register const reg(value);

    my_pulse1_channel.enable(reg.is_pulse1_channel_enabled());
    my_pulse2_channel.enable(reg.is_pulse2_channel_enabled());
    my_triangle_channel.enable(reg.is_triangle_channel_enabled());
    my_noise_channel.enable(reg.is_noise_channel_enabled());
}

/** Writes data into the frame counter register. */
void vapu::store_frame_counter(std::uint8_t value) noexcept {
    frame_counter_register const reg(value);

    my_frame_sequencer = reg.make_frame_sequencer();
    if (std::holds_alternative<five_step_sequencer>(my_frame_sequencer)) {
        clock_quarter_step();
        clock_half_step();
    }
}

void vapu::step_frame_counter() {
    std::visit([this](auto& counter) {
        counter.increment();
        step_frame_counter(counter);
    }, my_frame_sequencer);
}

void vapu::step_frame_counter(four_step_sequencer& counter) {
    clock_quarter_step();
    if (counter.value() == 1 || counter.value() == 3)
        clock_half_step();
}

void vapu::step_frame_counter(five_step_sequencer& counter) {
    if (counter.value() == 3)
        return;

    clock_quarter_step();
    if (counter.value() == 1 || counter.value() == 4)
        clock_half_step();
}

void vapu::clock_quarter_step() noexcept {
    my_pulse1_channel.clock_envelope();
    my_pulse2_channel.clock_envelope();

    my_triangle_channel.clock_linear_counter();
    my_noise_channel.clock_envelope();

    my_controller.clock_turbo();
}

void vapu::clock_half_step() noexcept {
    my_pulse1_channel.clock_length();
    my_pulse1_channel.clock_sweep<sweep_arithmetic_mode::ones_complement>();

    my_pulse2_channel.clock_length();
    my_pulse2_channel.clock_sweep<sweep_arithmetic_mode::twos_complement>();

    my_triangle_channel.clock_length();
    my_noise_channel.clock_length();
}

} // namespace emu::apu
