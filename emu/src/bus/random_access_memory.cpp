#include "emu/bus/random_access_memory.h"

#include "emu/address/raw_address.h"
#include "emu/address/address_mask.h"

#include <cstddef>
#include <cstdint>

namespace emu {

/** Reads a byte from RAM. */
std::uint8_t random_access_memory::load(abstract_address addr) const noexcept {
    abstract_address const mirrored_addr = mirrored_address(addr);
    return my_data[mirrored_addr.to_uint()];
}

/** Writes a byte to RAM. */
void random_access_memory::store(abstract_address addr, std::uint8_t value) noexcept {
    abstract_address const mirrored_addr = mirrored_address(addr);
    my_data[mirrored_addr.to_uint()] = value;
}

/** Returns a 256-byte span representing one RAM page. */
auto random_access_memory::page(page_index page_idx) const noexcept -> page_span {
    abstract_address const mirrored_first_addr = mirrored_address(abstract_address(page_idx));

    return page_span(my_data.begin() + mirrored_first_addr.to_uint(), page_size);
}

/** Returns a mirrored address in the canonical RAM range $0000-$07FF. */
abstract_address random_access_memory::mirrored_address(abstract_address addr) noexcept {
    constexpr auto addr_mask = address_mask<abstract_address>(ram_size - 1);
    return addr & addr_mask;
}

} // namespace emu
