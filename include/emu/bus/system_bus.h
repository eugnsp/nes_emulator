#pragma once

#include "emu/address/absolute_address.h"
#include "emu/types.h"

#include <concepts>

namespace emu {

template<class SystemBus>
concept system_bus = requires(SystemBus bus, absolute_address addr) {
    { bus.load_byte(addr) } -> std::same_as<uint8_t>;
    { bus.load_word(addr) } -> std::same_as<uint16_t>;
};

} // namespace emu
