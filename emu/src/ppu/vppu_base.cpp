#include "emu/ppu/vppu_base.h"

#include "emu/ppu/register/scroll_register.h"

#include <cstdint>

namespace emu::ppu {

std::uint8_t vppu_base::load_status() noexcept {
    status_register old_status_reg = my_status_reg;
    my_status_reg.clear_vblank();
    my_latch_reg.reset();

    old_status_reg.set_open_bus_bits(my_open_bus);
    return old_status_reg.to_uint();
}

std::uint8_t vppu_base::load_oam_data() {
    return my_oam.get(my_oam_addr_reg);
}

///////////////////////////////////////////////////////////////////////////////

void vppu_base::store_ctrl(std::uint8_t value) noexcept {
    control_register const old_control_reg = std::exchange(my_control_reg, control_register(value));

    my_t_reg.set_base_nametable_index(my_control_reg.base_nametable_index());
    if (my_status_reg.is_vblank_set() && my_control_reg.enable_vblank_nmi() > old_control_reg.enable_vblank_nmi())
        trigger_vblank_nmi();
}

void vppu_base::store_mask(std::uint8_t value) noexcept {
    my_mask_reg = mask_register(value);
}

void vppu_base::store_oam_address(std::uint8_t value) noexcept {
    my_oam_addr_reg = oam_address_register(value);
}

void vppu_base::store_oam_data(std::uint8_t value) {
    my_oam.set(my_oam_addr_reg++, value);
}

void vppu_base::store_scroll(std::uint8_t value) noexcept {
    scroll_register const scroll_reg(value);

    if (my_latch_reg.store_x()) {
        my_t_reg.set_coarse_x(scroll_reg.coarse());
        my_fine_x = scroll_reg.fine();
    }
    else {
        my_t_reg.set_coarse_y(scroll_reg.coarse());
        my_t_reg.set_fine_y(scroll_reg.fine());
    }

    my_latch_reg.toggle();
}

void vppu_base::store_address(std::uint8_t value) noexcept {
    if (my_latch_reg.store_hi())
        my_t_reg.set_hi_byte(value);
    else {
        my_t_reg.set_lo_byte(value);
        my_v_reg = my_t_reg;
    }

    my_latch_reg.toggle();
}

void vppu_base::store_oam_data_dma(oam_data_span oam_data) noexcept {
    my_oam.set(oam_data);
}

///////////////////////////////////////////////////////////////////////////////

void vppu_base::trigger_vblank_nmi() {
    my_cpu->set_nmi_flag();
}

abstract_address vppu_base::mirrored_register_address(abstract_address addr) noexcept {
    return (addr & register_addr_index_mask) | register_addr_base_mask;
}

} // namespace emu::ppu
