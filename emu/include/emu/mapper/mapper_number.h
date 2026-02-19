#pragma once

#include <cstdint>
#include <string>

namespace emu::mapper {

/** Mapper numbers corresponding to supported mapper (cartridge) types. */
enum class mapper_number : std::uint8_t {};

/** Returns a human-readable mapper name. */
std::string to_string(mapper_number);

} // namespace emu::map
