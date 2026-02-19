#pragma once

#include "emu/address/abstract_address.h"
#include "emu/bus/bus_word_access_mixin.h"
#include "emu/utility/dynamic_array.h"

#include <concepts>
#include <cstdint>
#include <ranges>
#include <stdexcept>

namespace emu::bus {

/**
 * Minimal bus that provides only memory access. The lower/upper part of the address space maps to RAM/ROM, respectively.
 */
class memory_only_bus : public detail::bus_word_access_mixin {
public:
    static constexpr std::uint32_t memory_size = 0x10000;  // Full 64 KB address space
    using memory_array = dynamic_array<std::uint8_t, memory_size>;

public:
    /** Constructs a bus with a reference to memory array; the first ram_size bytes are RAM, the remaining bytes are ROM. */
    memory_only_bus(memory_array& mem, std::uint32_t ram_size = memory_size) : my_memory(mem), my_ram_size(ram_size) {}

    /** Reads a byte from memory. */
    std::uint8_t load(abstract_address addr) const noexcept {
        return my_memory[addr.to_uint()];
    }

    /** Writes a byte to the RAM region; writes to the ROM region are ignored. */
    void store(abstract_address addr, std::uint8_t value) const noexcept {
        if (addr.to_uint() < my_ram_size)
            my_memory[addr.to_uint()] = value;
    }

    /** Copies the provided range of bytes into the RAM region. Throws if the range size exceeds the RAM capacity. */
    template<std::ranges::sized_range Range> requires std::same_as<std::ranges::range_value_t<Range>, std::uint8_t>
    void set_ram_bytes(Range&& range) const {
        if (std::ranges::size(range) > my_ram_size)
            throw std::invalid_argument("memory_only_bus: RAM size exceeds memory capacity");
        std::ranges::copy(range, my_memory.begin());
    }

    /** Copies the provided range of bytes into the ROM region. Throws if the range size exceeds the ROM capacity. */
    template<std::ranges::sized_range Range> requires std::same_as<std::ranges::range_value_t<Range>, std::uint8_t>
    void set_rom_bytes(Range&& range) const {
        if (std::ranges::size(range) + my_ram_size > memory_array::size())
            throw std::invalid_argument("memory_only_bus: ROM size exceeds memory capacity");
        std::ranges::copy(range, my_memory.begin() + my_ram_size);
    }

private:
    memory_array& my_memory;
    std::uint32_t my_ram_size;
};

} // namespace emu::bus
