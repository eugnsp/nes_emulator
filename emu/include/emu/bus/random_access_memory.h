#pragma once

#include "emu/address/abstract_address.h"
#include "emu/address/page.h"
#include "emu/constants.h"
#include "emu/utility/dynamic_array.h"

#include <cstddef>
#include <cstdint>
#include <span>

namespace emu {

/**
 * Random access memory (RAM); all addresses are mirrored into the $0000–$07FF range (2 KB).
 */
class random_access_memory {
public:
    static constexpr std::uint16_t ram_size = 0x0800;  // = 2 KB
    using page_span = std::span<std::uint8_t const, page_size>;

public:
    /** Reads a byte from RAM. */
    std::uint8_t load(abstract_address) const noexcept;

    /** Writes a byte to RAM. */
    void store(abstract_address, std::uint8_t) noexcept;

    /** Returns a 256-byte span representing one RAM page. */
    page_span page(page_index) const noexcept;

private:
    /** Returns a mirrored address in the canonical RAM range $0000-$07FF. */
    static abstract_address mirrored_address(abstract_address) noexcept;

private:
    dynamic_array<std::uint8_t, ram_size> my_data;
};

} // namespace emu
