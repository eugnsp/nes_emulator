#pragma once

#include "emu/fwd.h"
#include "emu/ppu/frame_buffer.h"
#include "emu/ppu/register/control_register.h"
#include "emu/ppu/register/mask_register.h"
#include "emu/ppu/register/status_register.h"
#include "emu/ppu/scanline_sprite.h"
#include "emu/ppu/vppu_base.h"
#include "emu/ppu/vram/vram.h"
#include "emu/utility/static_vector.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <ranges>
#include <utility>
#include <vector>

namespace emu::ppu {

template<class Mapper>
class vppu : public vppu_base {
public:
    vppu(Mapper&, nametable_mirroring, frame_buffer&);

    /** Reads a byte from a PPU register. */
    std::uint8_t load(abstract_address);

    /** Writes a byte to a PPU register. */
    void store(abstract_address, std::uint8_t);

    void step() {
        if (my_scanline.is_visible() && my_cycles.is_visible())
            render_pixel();

        if (my_mask_reg.show_background() || my_mask_reg.show_sprites()) {
            if (my_scanline.is_render() && my_cycles.is_bg_fetch()) {
                std::copy(my_tile_data.begin() + 1, my_tile_data.end(), my_tile_data.begin());

                switch (my_cycles.value() % 8) {
                case 1:
                    fetch_nametable_byte();
                    break;

                case 3:
                    fetch_attribute_table_byte();
                    break;

                case 5:
				    fetch_tile_bytes();
                    break;

                case 0:
                    store_tile_data();
                }
            }

            if (my_scanline.is_pre_render() && my_cycles.value() >= 280 && my_cycles.value() <= 304)
                my_v_reg.copy_y(my_t_reg);

            if (my_scanline.is_render()) {
                if (my_cycles.is_bg_fetch() && my_cycles.value() % 8 == 0)
                    my_v_reg.increment_x();
                if (my_cycles.value() == 256)
                    my_v_reg.increment_y();
                if (my_cycles.value() == 257)
                    my_v_reg.copy_x(my_t_reg);
            }

            if (my_scanline.is_render() && my_cycles.value() == 257) {
                if (my_scanline.is_visible())
                    evaluate_sprites();
                else
                    my_sprites.clear();
            }
        }

        if (my_scanline.value() == 241 && my_cycles.value() == 1) {
            my_pixel_buff.release();
            my_status_reg.set_vblank();
            if (my_control_reg.enable_vblank_nmi())
                trigger_vblank_nmi();
        }

        if (my_scanline.is_pre_render() && my_cycles.value() == 1) {
            my_status_reg.clear_vblank();
            my_status_reg.clear_sprite_zero_hit();
            my_status_reg.clear_sprite_overflow();
        }

        if (my_cycles.increment())
            my_scanline.increment();
    }

private:
    std::tuple<bool, sprite_priority, color_index> sprite_pixel() {
        if (!my_mask_reg.show_sprites())
            return {false, sprite_priority::behind, color_index{0}};

        for (scanline_sprite const& sprite : my_sprites) {
		    auto offset = (my_cycles.value() - 1) - sprite.position_x;
		    if (offset < 0 || offset > 7)
			    continue;
		    offset = 7 - offset;
		    color_index color = sprite.pattern[offset];
		    if (is_transparent(color))
                continue;
		    return {sprite.zero_index, sprite.priority, color};
	    }

        return {false, sprite_priority::behind, color_index{0}};
    }

    void render_pixel() {
        std::uint16_t const x = my_cycles.value() - 1;

        color_index background = {};
        if (my_mask_reg.show_background()) {
	        background = my_tile_data[my_fine_x];
        }

        auto [zero_index, priority, sprite] = sprite_pixel();

        if (x < 8) {
            if (!my_mask_reg.show_background_left8())
		        background = {};
	        if (!my_mask_reg.show_sprites_left8())
		        sprite = {};
	    }

        color_index color;

        bool const b = is_transparent(background);
	    bool const s = is_transparent(sprite);
	    if (b && s) {
	    	color = color_index::backdrop;
	    } else if (b && !s) {
	    	color = sprite;
	    } else if (!b && s) {
	    	color = background;
	    } else {
	    	if (zero_index && x < 255)
                my_status_reg.set_sprite_zero_hit();
	    	color = (priority == sprite_priority::in_front ? sprite : background);
	    }

        system_color_index const sys_color_idx = my_vram.my_palette[color];
        my_pixel_buff.set_next(sys_color_idx, my_mask_reg.color_effects());
    }

    void fetch_nametable_byte() {
        vram_address const addr = my_v_reg.nametable_address();
        my_nametable_byte = tile_index{my_vram.load(addr, my_open_bus)};
    }

    void fetch_attribute_table_byte() {
        std::uint16_t v = my_v_reg.value();
	    std::uint16_t address = 0x23C0 | (v & 0x0C00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07);
	    std::uint16_t shift = ((v >> 4) & 4) | (v & 2);

        auto a = my_vram.load(vram_address{address}, my_open_bus);
        my_attribute_table_byte = color_index{static_cast<std::uint8_t>(((a >> shift) & 3) << 2)};
    }

    void fetch_tile_bytes() {
        std::uint8_t const fine_y = my_v_reg.fine_y();
	    pattern_table_index table_idx = my_control_reg.bg_pattern_table_index();
	    auto tile = my_nametable_byte;
        my_bg_tile = my_vram.load_tile_row(table_idx, tile, fine_y);
    }

    void evaluate_sprites() {
        switch (my_control_reg.sprite_size()) {
            case sprite_size_mode::s8x8:
                evaluate_sprites_impl<sprite_size_mode::s8x8>();
                return;

            case sprite_size_mode::s8x16:
                evaluate_sprites_impl<sprite_size_mode::s8x16>();
                return;
        }

        assert(false && "Unreachable");
        std::unreachable();
    }

    template<class T, T lo, T hi, class S>
    std::optional<T> try_bound(S value) {
        if (std::cmp_less_equal(lo, value) && std::cmp_less(value, hi))
            return static_cast<T>(value);
        return {};
    }

    template<sprite_size_mode SpriteSize>
    void evaluate_sprites_impl() {
        my_sprites.clear();
        for (auto const& [sprite_idx, sprite] : std::views::enumerate(my_oam)) {
            constexpr std::uint8_t height = sprite_height(SpriteSize);
            auto const row = try_bound<std::uint8_t, 0, height>(my_scanline.value() - sprite.position_y);
            if (!row.has_value())
                continue;

            if (my_sprites.is_full()) {
                my_status_reg.set_sprite_overflow();
                break;
            }

            my_sprites.push_back(scanline_sprite{
                .position_x = sprite.x_position,
                .priority   = sprite.attributes.priority(),
                .zero_index = (sprite_idx == 0),
                .pattern    = fetch_sprite_pattern<SpriteSize>(sprite, *row)
            });
        }
    }

    template<sprite_size_mode SpriteSize>
    tile_row fetch_sprite_pattern_impl(oam_entry const& sprite, int row) {
        auto tile_number = sprite.index;

	    if constexpr (SpriteSize == sprite_size_mode::s8x8) {
		    pattern_table_index const table_idx = my_control_reg.sprite_pattern_table_index();
		    if (sprite.attributes.flip_vert())
			    row = 7 - row;

	        return my_vram.load_tile_row(table_idx, tile_number.index_8x8(), row);
	    } else {
		    pattern_table_index const table_idx = tile_number.sprite_pt_table_addr_8x16();
		    if (sprite.attributes.flip_vert())
			    row = 15 - row;
		    auto tile_idx = tile_number.index_8x16();
		    if (row > 7) {
			    tile_idx = tile_index{static_cast<std::uint8_t>(std::to_underlying(tile_idx) + 1)};
                row -= 8;
		    }

	        return my_vram.load_tile_row(table_idx, tile_idx, row);
	    }
    }

    template<sprite_size_mode SpriteSize>
    std::array<color_index, 8> fetch_sprite_pattern(oam_entry const& sprite, int row) {
        tile_row const tile = fetch_sprite_pattern_impl<SpriteSize>(sprite, row);

        std::array<color_index, 8> pattern;
        tile.decode(sprite.attributes.palette(), pattern, sprite.attributes.flip_horz());

        return pattern;
    }

private:
    /** Reads a byte from VRAM memory. */
    std::uint8_t load_vram();

    /** Writes a byte to VRAM memory. */
    void store_vram(std::uint8_t);

private:
    Mapper& my_mapper;
    vram<Mapper> my_vram;

    frame_buffer& my_pixel_buff;

    /** The prefetched data for sprite rows on the current scanline. */
    static_vector<scanline_sprite, max_sprites_per_scanline> my_sprites;

    /** Internal read buffer holding the last nametable/pattern byte for CPU read delay. */
    std::uint8_t my_read_buff = {};
};

} // namespace emu::ppu

#include "emu/cpu/vcpu.h"
#include "emu/mapper.h"

#include <cassert>
#include <iostream>
#include <format>
#include <chrono>
#include <thread>

namespace emu::ppu {

template<class Mapper>
vppu<Mapper>::vppu(Mapper& mapper, nametable_mirroring mirroring, frame_buffer& pixel_buff) :
    my_mapper(mapper), my_vram(mapper, mirroring), my_pixel_buff(pixel_buff) {}

/** Reads a byte from a PPU register. */
template<class Mapper>
std::uint8_t vppu<Mapper>::load(abstract_address addr) {
    abstract_address const mirrored_addr = mirrored_register_address(addr);
    if (mirrored_addr == status_register_addr)
        my_open_bus = load_status();
    else if (mirrored_addr == oam_data_register_addr)
        my_open_bus = load_oam_data();
    else if (mirrored_addr == data_register_addr)
        my_open_bus = load_vram();

    return my_open_bus;
}

/** Writes a byte to a PPU register. */
template<class Mapper>
void vppu<Mapper>::store(abstract_address addr, std::uint8_t value) {
    my_open_bus = value;

    abstract_address const mirrored_addr = mirrored_register_address(addr);
    if (mirrored_addr == control_register_addr)
        store_ctrl(value);
    else if (mirrored_addr == mask_register_addr)
        store_mask(value);
    else if (mirrored_addr == oam_addr_register_addr)
        store_oam_address(value);
    else if (mirrored_addr == oam_data_register_addr)
        store_oam_data(value);
    else if (mirrored_addr == scroll_register_addr)
        store_scroll(value);
    else if (mirrored_addr == address_register_addr)
        store_address(value);
    else if (mirrored_addr == data_register_addr)
        store_vram(value);
}

/** Reads a byte from VRAM memory. */
template<class Mapper>
void vppu<Mapper>::store_vram(std::uint8_t value) {
    vram_address const addr = my_v_reg.to_vram_address();
    my_v_reg.increment(my_control_reg.vram_address_increment());

    my_vram.store(addr, value);
}

/** Writes a byte to VRAM memory. */
template<class Mapper>
std::uint8_t vppu<Mapper>::load_vram() {
    vram_address const addr = my_v_reg.to_vram_address();
    my_v_reg.increment(my_control_reg.vram_address_increment());

    std::uint8_t const value = my_vram.load(addr, my_open_bus);

    if (addr < vram_palette_addr) {
        // For pattern/nametable reads, the CPU should see a delayed value
        return std::exchange(my_read_buff, value);
    }
    else {
        // For palette reads, the internal read buffer should be filled with the mirrored nametable byte
        vram_address const mirrored_addr = addr & vram_nametable_addr_mask;
        my_read_buff = my_vram.load(mirrored_addr, my_open_bus);

        return value;
    }
}

} // namespace emu::ppu
