#include "emu/apu/register/frame_counter_register.h"

#include "emu/apu/types.h"

#include <iostream>

namespace emu::apu {

frame_sequencer frame_counter_register::make_frame_sequencer() const noexcept {
    return is_set<flags::counter_mode>() ?
        frame_sequencer(std::in_place_type<five_step_sequencer>) :
        frame_sequencer(std::in_place_type<four_step_sequencer>);
}

} // namespace emu::apu
