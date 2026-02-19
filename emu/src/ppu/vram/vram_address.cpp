#include "emu/ppu/vram/vram_address.h"

#include "emu/utility/bit_ops.h"

#include <cstddef>
#include <cstdint>

namespace emu::ppu {

/** Sets the low byte of the VRAM address. */
void vram_address::set_lo_byte(std::uint8_t byte) noexcept {
    my_addr = to_word(byte, get_hi_byte(my_addr));
}

/** Sets the high byte of the VRAM address, ignoring bits 14 and 15. */
void vram_address::set_hi_byte(std::uint8_t byte) noexcept {
    constexpr std::uint8_t hi_byte_mask = get_hi_byte(mask);
    my_addr = to_word(get_lo_byte(my_addr), byte & hi_byte_mask);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Returns a VRAM address advanced by the given offset (wrapping within the 14-bit address space). */
vram_address operator+(vram_address addr, vram_address::difference_type n) noexcept {
    return vram_address(addr.to_uint() + n);
}

/** Returns the signed distance between two VRAM addresses. */
vram_address::difference_type operator-(vram_address addr1, vram_address addr2) noexcept {
    using diff = vram_address::difference_type;
    return static_cast<diff>(addr1.to_uint()) - static_cast<diff>(addr2.to_uint());
}

} // namespace emu::ppu
