#include "emu/ppu/register/loopy_register.h"

namespace emu::ppu {

/** Returns a 14-bit VRAM address represented by the register. */
vram_address loopy_register::to_vram_address() const noexcept {
    return vram_address(to_uint());
}

vram_address loopy_register::nametable_address() const noexcept {
    constexpr flags mask = bitwise_or(flags::nametable, flags::coarse_y, flags::coarse_x);
    return vram_nametable_addr + to_uint_masked<mask>();
}

/** Returns a fine Y scroll value. */
std::uint8_t loopy_register::fine_y() const noexcept {
    return to_uint_masked<flags::fine_y>();
}

void loopy_register::set_base_nametable_index(nametable_index idx) noexcept {
    set_masked_uint<flags::nametable>(std::to_underlying(idx));
}

void loopy_register::copy_x(loopy_register const& other) noexcept {
    constexpr auto mask = std::to_underlying(bitwise_or(flags::nametable_x, flags::coarse_x));
    my_flags = (my_flags & ~mask) | (other.my_flags & mask);
}

void loopy_register::copy_y(loopy_register const& other) noexcept {
    constexpr auto mask = std::to_underlying(bitwise_or(flags::fine_y, flags::nametable_y, flags::coarse_y));
    my_flags = (my_flags & ~mask) | (other.my_flags & mask);
}

void loopy_register::increment_x() noexcept {
    std::uint8_t new_coarse_x = to_uint_masked<flags::coarse_x>() + 1;
    if (new_coarse_x == 32) {
        flip<flags::nametable_x>();
        new_coarse_x = 0;
    }

    set_masked_uint<flags::coarse_x>(new_coarse_x);
}

void loopy_register::increment_y() noexcept {
    if (!are_all_set<flags::fine_y>()) { // TODO
        set_masked_uint<flags::fine_y>(to_uint_masked<flags::fine_y>() + 1);
        return;
    }

    set_masked_uint<flags::fine_y>(0);

    std::uint8_t coarse_y = to_uint_masked<flags::coarse_y>() + 1;
    if (coarse_y == 30) {
        flip<flags::nametable_y>();
        coarse_y = 0;
    } else if (coarse_y == 32)
        coarse_y = 0;

    set_masked_uint<flags::coarse_y>(coarse_y);
}

void loopy_register::increment(underlying_type n) noexcept {
    my_flags += n;
}

void loopy_register::store_address(latch_register latch_reg, std::uint8_t value) noexcept {
    constexpr std::uint8_t hi_byte_mask = std::uint8_t{0b0011'1111};

    if (latch_reg.store_hi())
        set_masked_uint<flags::hi_byte>(value & hi_byte_mask);
    else
        set_masked_uint<flags::lo_byte>(value);
}

void loopy_register::store_scroll(latch_register latch_reg, scroll_register scroll_reg) noexcept {
    if (latch_reg.store_x())
        set_masked_uint<flags::coarse_x>(scroll_reg.coarse());
    else {
        set_masked_uint<flags::coarse_y>(scroll_reg.coarse());
        set_masked_uint<flags::fine_y>(scroll_reg.fine());
    }
}

void loopy_register::set_coarse_x(std::uint8_t value) noexcept {
    assert(value < 32);
    set_masked_uint<flags::coarse_x>(value);
}

void loopy_register::set_coarse_y(std::uint8_t value) noexcept {
    assert(value < 32);
    set_masked_uint<flags::coarse_y>(value);
}

void loopy_register::set_fine_y(std::uint8_t value) noexcept {
    assert(value < 8);
    set_masked_uint<flags::fine_y>(value);
}

/** Sets the high byte of the register. */
void loopy_register::set_hi_byte(std::uint8_t byte) noexcept {
    constexpr std::uint8_t hi_byte_mask = std::uint8_t{0b0011'1111};
    set_masked_uint<flags::hi_byte>(byte & hi_byte_mask);
}

/** Sets the low byte of the register. */
void loopy_register::set_lo_byte(std::uint8_t byte) noexcept {
    set_masked_uint<flags::lo_byte>(byte);
}

} // namespace emu::ppu
