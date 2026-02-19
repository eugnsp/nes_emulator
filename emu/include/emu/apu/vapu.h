#pragma once

#include "emu/address.h"
#include "emu/apu/noise_channel.h"
#include "emu/apu/pulse_channel.h"
#include "emu/apu/triangle_channel.h"
#include "emu/apu/types.h"
#include "emu/fwd.h"
#include "emu/utility/cyclic_counter.h"

#include <cstddef>
#include <cstdint>

namespace emu::apu {

class vapu {
public:
    vapu(vcontroller&);

    /** Returns the current output of the APU in the range [0, 1]. */
    float output();

    /** Reads a byte from an APU register. */
    std::uint8_t load(abstract_address) noexcept;

    /** Writes a byte to an APU register. */
    void store(abstract_address, std::uint8_t) noexcept;

    void step(std::uint64_t cpu_cycles) {
        for (std::uint64_t i = 0; i < cpu_cycles; ++i)
        {
            if (my_odd_cycle)
            {
                my_pulse1_channel.clock_timer();
                my_pulse2_channel.clock_timer();
                my_noise_channel.clock_timer();
            }
            my_odd_cycle = !my_odd_cycle;

            my_triangle_channel.clock_timer();
            if (frame_counter0.increment())
                step_frame_counter();
        }
    }

private:
    /** Reads data from the status register. */
    std::uint8_t load_status() const noexcept;

    /** Writes data into the status register. */
    void store_status(std::uint8_t) noexcept;

    /** Writes data into the frame counter register. */
    void store_frame_counter(std::uint8_t) noexcept;

    void step_frame_counter();

    void step_frame_counter(four_step_sequencer&);
    void step_frame_counter(five_step_sequencer&);

    void clock_quarter_step() noexcept;

    void clock_half_step() noexcept;

private:
    /** APU address space. */
    static constexpr auto apu_p1_ctrl_register      = abstract_address{0x4000};  // Write only
    static constexpr auto apu_p1_sweep_register     = abstract_address{0x4001};  // Write only
    static constexpr auto apu_p1_timer_lo_register  = abstract_address{0x4002};  // Write only
    static constexpr auto apu_p1_timer_hi_register  = abstract_address{0x4003};  // Write only
    static constexpr auto apu_p2_ctrl_register      = abstract_address{0x4004};  // Write only
    static constexpr auto apu_p2_sweep_register     = abstract_address{0x4005};  // Write only
    static constexpr auto apu_p2_timer_lo_register  = abstract_address{0x4006};  // Write only
    static constexpr auto apu_p2_timer_hi_register  = abstract_address{0x4007};  // Write only
    static constexpr auto apu_tri_control_register  = abstract_address{0x4008};
    static constexpr auto apu_tri_timer_lo_register = abstract_address{0x400A};
    static constexpr auto apu_tri_timer_hi_register = abstract_address{0x400B};
    static constexpr auto apu_noise_ctrl_register   = abstract_address{0x400C};  // Write only
    static constexpr auto apu_noise_mode_register   = abstract_address{0x400E};  // Write only
    static constexpr auto apu_noise_timer_register  = abstract_address{0x400F};  // Write only
    static constexpr auto apu_status_register       = abstract_address{0x4015};  // Write only
    static constexpr auto apu_ctrl1_register        = abstract_address{0x4016};  // Read/Write
    static constexpr auto apu_ctrl2_register        = abstract_address{0x4017};  // Read only
    static constexpr auto apu_fc_register           = abstract_address{0x4017};  // Write only

private:
    static constexpr std::uint16_t cycles_per_frame = 7'457;
    cyclic_counter<std::uint16_t, cycles_per_frame> frame_counter0;

    pulse_channel    my_pulse1_channel;
    pulse_channel    my_pulse2_channel;
    triangle_channel my_triangle_channel;
    noise_channel    my_noise_channel;

    bool my_odd_cycle = false;

    vcontroller& my_controller;
    frame_sequencer my_frame_sequencer;
};

} // namespace emu::apu
