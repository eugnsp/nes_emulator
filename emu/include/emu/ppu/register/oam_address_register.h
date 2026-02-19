#pragma once

#include <cstddef>
#include <cstdint>

namespace emu::ppu {

/** PPU OAM address register ($2003, write only) - Sprite RAM address. */
class oam_address_register {
public:
    oam_address_register() = default;
    oam_address_register(std::uint8_t) noexcept;

    /** Returns the underlying numeric value. */
    std::uint8_t to_uint() const noexcept;

    /** Post-increment operator, advances the address after returning current value. */
    oam_address_register operator++(int) noexcept;

private:
    std::uint8_t my_addr = 0;
};

} // namespace emu::ppu
