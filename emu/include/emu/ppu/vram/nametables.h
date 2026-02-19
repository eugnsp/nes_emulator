#pragma once

#include "emu/ppu/types.h"
#include "emu/ppu/vram/vram_address.h"
#include "emu/utility/dynamic_array.h"

#include <array>
#include <cstddef>
#include <cstdint>

namespace emu::ppu {

/** Class that provides access to logical nametables backed by physical CIRAM/cartridge memory. */
class nametables {
public:
    explicit nametables(nametable_mirroring);

    /** Reads a byte from the nametable address range. */
    std::uint8_t load(vram_address) const noexcept;

    /** Writes a byte to the nametable address range. */
    void store(vram_address, std::uint8_t value) noexcept;

    /** Updates the mapping of logical nametables. */
    void set_mirroring(nametable_mirroring) noexcept;

private:
    using base_pointers = std::array<std::uint8_t*, num_nametables>;
    using ciram_storage = dynamic_array<std::uint8_t, num_nametables * nametable_size>;

private:
    struct decode_result {
        std::uint8_t  table_index;      // Valid range: [0, num_nametables)
        std::uint16_t offset_in_table;  // Valid range: [0, nametable_size)
    };

    /**
    * Decodes an address in the nametable address range into a logical
    * nametable index and an offset within the corresponding table.
    */
    static decode_result decode_nametable_address(vram_address) noexcept;

    /** Returns pointers to the start of each logical nametable. */
    base_pointers base_ptrs(nametable_mirroring) noexcept;

private:
    base_pointers my_base_pointers;  // Pointers to the start of each logical nametable
    ciram_storage my_ciram;          // Physical memory for nametables (4KB)
};

} // namespace emu::ppu
