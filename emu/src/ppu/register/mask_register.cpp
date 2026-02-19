#include "emu/ppu/register/mask_register.h"

namespace emu::ppu {

/** Returns true if background is shown in leftmost 8 pixels. */
bool mask_register::show_background_left8() const noexcept {
    return is_set<flags::show_background_left>();
}

/** Returns true if sprites are shown in leftmost 8 pixels. */
bool mask_register::show_sprites_left8() const noexcept {
    return is_set<flags::show_sprites_left>();
}

/** Returns true if background rendering is enabled. */
bool mask_register::show_background() const noexcept {
    return is_set<flags::show_background>();
}

/** Returns true if sprite rendering is enabled. */
bool mask_register::show_sprites() const noexcept {
    return is_set<flags::show_sprites>();
}

/** Returns the color effects (grayscale and RGB emphasis states). */
color_effects_flags mask_register::color_effects() const noexcept {
    return color_effects_flags{
        .grayscale       = is_set<flags::grayscale>(),
        .emphasize_red   = is_set<flags::emphasize_red>(),
        .emphasize_green = is_set<flags::emphasize_green>(),
        .emphasize_blue  = is_set<flags::emphasize_blue>()
    };
}

} // namespace emu::ppu
