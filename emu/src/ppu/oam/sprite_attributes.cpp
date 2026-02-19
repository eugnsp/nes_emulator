#include "emu/ppu/oam/sprite_attributes.h"

#include "emu/ppu/types.h"
#include "emu/utility/bit_ops.h"

namespace emu::ppu {

/** Returns the sprite palette base offset in palette RAM. */
color_index sprite_attributes::palette() const noexcept {
    return bitwise_or(
        color_index{static_cast<std::uint8_t>(to_uint_masked<flags::palette>() << 2)},
        color_index::sprite_mask
    );
}

/** Returns whether the sprite appears in front of or behind the background. */
sprite_priority sprite_attributes::priority() const noexcept {
    return is_set<flags::priority>() ?
        sprite_priority::behind : sprite_priority::in_front;
}

/** Returns true if the sprite is flipped horizontally. */
bool sprite_attributes::flip_horz() const noexcept {
    return is_set<flags::flip_horz>();
}

/** Returns true if the sprite is flipped vertically. */
bool sprite_attributes::flip_vert() const noexcept {
    return is_set<flags::flip_vert>();
}

/** Clears unimplemented bits that should always read as zero. */
void sprite_attributes::clear_unimplemented_bits() noexcept {
    reset<flags::unimplemented>();
}

} // namespace emu::ppu
