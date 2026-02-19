#pragma once

#include "emu/address.h"
#include "emu/address/address_mask.h"
#include "emu/constants.h"
#include "emu/cpu/vcpu_state.h"
#include "emu/ppu/cycle_counter.h"
#include "emu/ppu/oam/oam_table.h"
#include "emu/ppu/frame_buffer.h"
#include "emu/ppu/register/control_register.h"
#include "emu/ppu/register/latch_register.h"
#include "emu/ppu/register/loopy_register.h"
#include "emu/ppu/register/mask_register.h"
#include "emu/ppu/register/oam_address_register.h"
#include "emu/ppu/register/status_register.h"
#include "emu/ppu/scanline_counter.h"
#include "emu/ppu/types.h"
#include "emu/ppu/vram/vram_address.h"
#include "emu/utility/dynamic_array.h"

#include <array>
#include <cstdint>
#include <cstddef>
#include <span>

namespace emu::ppu {

class vppu_base {
public:
    /** View into object attribute memory. */
    using oam_data_span = std::span<std::uint8_t const, ppu_oam_size>;

    void set_cpu(cpu::vcpu_state& cpu) {
        my_cpu = &cpu;
    }

    /** Reads data from OAM. */
    std::uint8_t load_oam_data();

    /** Writes data into OAM using DMA. */
    void store_oam_data_dma(oam_data_span) noexcept;

protected:
    vppu_base() = default;

    ///////////////////////////////////////////////////////////////////////////
    /** Read operations. */

    /** Reads data from the status register. */
    std::uint8_t load_status() noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /** Write operations. */

    /** Writes data into the control register. */
    void store_ctrl(std::uint8_t) noexcept;

    /** Writes data into the mask register. */
    void store_mask(std::uint8_t) noexcept;

    /** Writes data into the OAM address register. */
    void store_oam_address(std::uint8_t) noexcept;

    /** Writes data into OAM. */
    void store_oam_data(std::uint8_t);

    /** Writes data into the scroll register. */
    void store_scroll(std::uint8_t) noexcept;

    /** Writes data into the address register. */
    void store_address(std::uint8_t) noexcept;

    void store_tile_data() {
        my_bg_tile.decode(my_attribute_table_byte, std::span<color_index, 8>(my_tile_data.begin() + 8, 8), true);
    }

public:
    void trigger_vblank_nmi();

protected:
    /** Returns a mirrored register address in the canonical PPU range $2000-$2007. */
    static abstract_address mirrored_register_address(abstract_address) noexcept;

protected:
    /** PPU address space, $2000-$3FFF: $2000–$2007 + 8-byte mirror ranges $2008–$3FFF. */
    static constexpr auto register_addr_base_mask   = address_mask<abstract_address>{0x2000};
    static constexpr auto register_addr_index_mask  = address_mask<abstract_address>{0x0007};

    static constexpr auto control_register_addr     = abstract_address{0x2000};  // Write only
    static constexpr auto mask_register_addr        = abstract_address{0x2001};  // Write only
    static constexpr auto status_register_addr      = abstract_address{0x2002};  // Read only
    static constexpr auto oam_addr_register_addr    = abstract_address{0x2003};  // Write only
    static constexpr auto oam_data_register_addr    = abstract_address{0x2004};  // Read/Write
    static constexpr auto scroll_register_addr      = abstract_address{0x2005};  // Write only
    static constexpr auto address_register_addr     = abstract_address{0x2006};  // Write only
    static constexpr auto data_register_addr        = abstract_address{0x2007};  // Read/Write

protected:
    cycle_counter    my_cycles;
    scanline_counter my_scanline;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /** Registers. */

    control_register     my_control_reg;   // $2000, write only
    mask_register        my_mask_reg;      // $2001, write only
    status_register      my_status_reg;    // $2002, read only
    oam_address_register my_oam_addr_reg;  // $2003, write only

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /** Internal registers. */

    latch_register my_latch_reg;
    loopy_register my_v_reg;
    loopy_register my_t_reg;
    std::uint8_t   my_fine_x = 0;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /** Random access memory. */

    ppu::oam_table my_oam;  // $2004, Object attribute memory

    cpu::vcpu_state* my_cpu = nullptr;

    std::uint8_t my_open_bus = {};  // Open bus: The last value read from a readable register or written to any register

    tile_index my_nametable_byte = {}; // = tile index latch
    color_index my_attribute_table_byte = color_index::backdrop;
    tile_row my_bg_tile = {};
    std::array<color_index, 16> my_tile_data;

    //pixel_matrix my_pixel_array;
};

} // namespace emu::ppu
