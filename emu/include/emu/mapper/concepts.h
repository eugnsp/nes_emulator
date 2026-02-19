#pragma once

#include "emu/address/raw_address.h"
#include "emu/constants.h"
#include "emu/ppu/vram/vram_address.h"

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <span>

namespace emu::mapper::concepts {

template<class Mapper>
concept mapper = requires(Mapper& mapper, abstract_address addr, ppu::vram_address vaddr, std::uint8_t value) {
    { mapper.load_prg(addr) } -> std::same_as<std::uint8_t>;
    mapper.store_prg(addr, value);

    { mapper.load_chr(vaddr) } -> std::same_as<std::uint8_t>;
    mapper.store_chr(vaddr, value);
};

} // namespace emu::map::concepts
