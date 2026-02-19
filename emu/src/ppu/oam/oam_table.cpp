#include "emu/ppu/oam/oam_table.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <span>

namespace emu::ppu {

/** Returns an iterator to the first OAM entry. */
auto oam_table::begin() const noexcept -> const_iterator {
    return std::begin(my_data);
}

/** Returns an iterator past the last OAM entry. */
auto oam_table::end() const noexcept -> const_iterator {
    return std::end(my_data);
}

/** Writes the entire OAM (usually as part of a DMA transfer) from raw bytes. */
void oam_table::set(const_span data) noexcept {
    std::ranges::copy(data, as_bytes().begin());
}

/** Reads a single byte from OAM. */
std::uint8_t oam_table::get(oam_address_register addr) const noexcept {
    std::uint8_t value = as_bytes()[addr.to_uint()];
    if (addr.to_uint() % sizeof(oam_entry) == offsetof(oam_entry, attributes))
        reinterpret_cast<sprite_attributes&>(value).clear_unimplemented_bits();

    return value;
}

/** Writes a single byte into OAM. */
void oam_table::set(oam_address_register addr, std::uint8_t value) noexcept {
    as_bytes()[addr.to_uint()] = value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

auto oam_table::as_bytes() noexcept -> span {
    return span(reinterpret_cast<std::uint8_t*>(&my_data), size_bytes);
}

auto oam_table::as_bytes() const noexcept -> const_span {
    return const_span(reinterpret_cast<std::uint8_t const*>(&my_data), size_bytes);
}

} // namespace emu
