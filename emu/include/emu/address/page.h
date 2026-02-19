#pragma once

#include <cstdint>

namespace emu {

/** An index of a 256-byte memory page. */
enum class page_index : std::uint8_t {};

/** Size of a single memory page, in bytes. */
inline constexpr std::uint16_t page_size = 0x0100;

} // namespace emu
