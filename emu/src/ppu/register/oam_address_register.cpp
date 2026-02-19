#include "emu/ppu/register/oam_address_register.h"

namespace emu::ppu {

oam_address_register::oam_address_register(std::uint8_t addr) noexcept : my_addr(addr) {}

/** Returns the underlying numeric value. */
std::uint8_t oam_address_register::to_uint() const noexcept {
    return my_addr;
}

/** Post-increment operator, advances the address after returning current value. */
oam_address_register oam_address_register::operator++(int) noexcept {
    oam_address_register const old = *this;
    ++my_addr;
    return old;
}

} // namespace emu::ppu
