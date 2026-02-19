#include "emu/ppu/register/status_register.h"

#include <cstddef>
#include <cstdint>

namespace emu::ppu {

/** Updates the open bus bits from a given open bus value. */
void status_register::set_open_bus_bits(std::uint8_t open_bus) noexcept {
    set_bits<flags::open_bus>(open_bus);
}

/** Clears the sprite overflow flag. */
void status_register::clear_sprite_overflow() noexcept {
    set<flags::sprite_overflow>(false);
}

/** Sets the sprite overflow flag. */
void status_register::set_sprite_overflow() noexcept {
    set<flags::sprite_overflow>(true);
}

/** Clears the sprite 0 hit flag. */
void status_register::clear_sprite_zero_hit() noexcept {
    set<flags::sprite_zero_hit>(false);
}

/** Sets the sprite 0 hit flag. */
void status_register::set_sprite_zero_hit() noexcept {
    set<flags::sprite_zero_hit>(true);
}

/** Returns true if the vblank flag is set. */
bool status_register::is_vblank_set() const noexcept {
    return is_set<flags::in_vblank>();
}

/** Clears the v-blank flag. */
void status_register::clear_vblank() noexcept {
    set<flags::in_vblank>(false);
}

/** Sets the v-blank flag. */
void status_register::set_vblank() noexcept {
    set<flags::in_vblank>(true);
}

} // namespace emu::ppu
