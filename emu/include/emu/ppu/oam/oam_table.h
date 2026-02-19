#pragma once

#include "emu/ppu/oam/oam_entry.h"
#include "emu/ppu/register/oam_address_register.h"

#include <cstddef>
#include <cstdint>
#include <span>

namespace emu::ppu {

/** OAM (object attribute memory) holds information about 64 sprites. Size of OAM is 256 bytes. */
class oam_table {
private:
    static constexpr std::uint16_t size_bytes = 0x0100;
    static constexpr std::uint8_t  num_sprites   = size_bytes / sizeof(oam_entry);

public:
    using span       = std::span<std::uint8_t,       size_bytes>;
    using const_span = std::span<std::uint8_t const, size_bytes>;

    using value_type     = oam_entry;
    using const_iterator = value_type const*;

public:
    /** Returns an iterator to the first OAM entry. */
    const_iterator begin() const noexcept;

    /** Returns an iterator past the last OAM entry. */
    const_iterator end() const noexcept;

    /** Writes the entire OAM (usually as part of a DMA transfer) from raw bytes. */
    void set(const_span) noexcept;

    /** Reads a single byte from OAM. */
    std::uint8_t get(oam_address_register) const noexcept;

    /** Writes a single byte into OAM. */
    void set(oam_address_register, std::uint8_t) noexcept;

private:
    span       as_bytes() noexcept;
    const_span as_bytes() const noexcept;

private:
    oam_entry my_data[num_sprites];
    static_assert(sizeof(my_data) == size_bytes);
};

} // namespace emu::ppu
