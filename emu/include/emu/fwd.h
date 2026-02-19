#pragma once

#include "emu/mapper/concepts.h"

#include <cstdint>
#include <type_traits>

namespace emu {

class absolute_address;

namespace cpu {
    class vcpu_state;

    template<class SystemBus>
    class vcpu;
}

namespace apu {
    class vapu;
}

template<mapper::concepts::mapper Mapper, class PPU, class APU>
class nes_system_bus;

namespace ppu {
    template<class Mapper>
    class vppu;
}

class vcontroller;

}
