#pragma once

#include "emu/utility/bit_ops.h"
#include "emu/utility/type_traits.h"

#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>

namespace emu {

template<class Enum, class T>
concept enum_with_underlying = std::is_enum_v<Enum> && std::same_as<std::underlying_type_t<Enum>, T>;

template<std::unsigned_integral T, class StorageType = T>
class bit_flags {
public:
    using underlying_type = T;

private:
    //template<Enum Mask>
    //using uint_type = smallest_size_type<(1 << std::popcount(std::to_underlying(Mask)))>;

public:
    bit_flags() = default;

    explicit bit_flags(underlying_type flags) noexcept : my_flags(flags) {}

    template<class Enum> requires enum_with_underlying<Enum, underlying_type>
    explicit bit_flags(Enum flags) noexcept : my_flags(std::to_underlying(flags)) {}

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Underlying value access

    /** Returns the underlying unsigned integral value. */
    constexpr underlying_type to_uint() const noexcept {
        return my_flags;
    }

    /** Set the underlying unsigned integral value. */
    constexpr bit_flags& from_uint(underlying_type flags) noexcept {
        my_flags = flags;
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Bits access

    /** Returns true if any bit from the mask is set. */
    template<class Mask> requires enum_with_underlying<Mask, underlying_type>
    bool is_any_set(Mask mask) const noexcept {
        return (my_flags & std::to_underlying(mask)) != 0;
    }

    /** Returns true if any bit from the compile-time mask is set. */
    template<auto Mask> requires enum_with_underlying<decltype(Mask), underlying_type>
    bool is_any_set() const noexcept {
        return is_any_set(Mask);
    }

    /** Returns true if all bits from the compile-time mask are set. */
    template<auto Mask> requires enum_with_underlying<decltype(Mask), underlying_type>
    bool are_all_set() const noexcept {
        return (my_flags & std::to_underlying(Mask)) == std::to_underlying(Mask);
    }

    /** Returns true if the single bit specified by the mask is set. */
    template<class Mask> requires enum_with_underlying<Mask, underlying_type>
    bool is_set(Mask mask) const noexcept {
        assert(has_single_bit(mask) && "Mask must have a single bit set");
        return is_any_set(mask);
    }

    /** Returns true if the single bit specified by the compile-time mask is set. */
    template<auto Mask> requires enum_with_underlying<decltype(Mask), underlying_type>
    bool is_set() const noexcept {
        static_assert(has_single_bit(Mask), "Mask must have a single bit set");
        return is_set(Mask);
    }

    /** Returns true if the bit at the given index is set. */
    bool is_nth_set(std::uint8_t bit_index) const noexcept {
        return is_set(nth_bit_mask(bit_index));
    }

    /** Extracts a masked value, shifted down to bit zero. */
    template<auto Mask> requires enum_with_underlying<decltype(Mask), underlying_type>
    underlying_type to_uint_masked() const {
        static_assert(has_contiguous_bits(Mask), "Mask must be contiguous");

        constexpr std::uint8_t pos = std::countr_zero(std::to_underlying(Mask));
        return static_cast<underlying_type>((my_flags & std::to_underlying(Mask)) >> pos);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    template<auto Mask> requires enum_with_underlying<decltype(Mask), underlying_type>
    void set_bits(underlying_type value) noexcept {
        constexpr underlying_type mask = std::to_underlying(Mask);
        my_flags = (my_flags & ~mask) | (value & mask);
    }

    template<auto Mask> requires enum_with_underlying<decltype(Mask), underlying_type>
    void set_masked_uint(underlying_type value) noexcept {
        static_assert(has_contiguous_bits(Mask), "Mask must be contiguous");
        constexpr std::size_t shift = std::countr_zero(std::to_underlying(Mask));

        underlying_type const mask = std::to_underlying(Mask);
        my_flags = (my_flags & ~mask) | static_cast<underlying_type>(value << shift);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifiers

    /** Sets the bits specified by the mask. */
    template<class Mask> requires enum_with_underlying<Mask, underlying_type>
    bit_flags& set(Mask mask, bool state = true) noexcept {
        if (state)
            my_flags |= std::to_underlying(mask);
        else
            my_flags &= ~std::to_underlying(mask);
        return *this;
    }

    /** Sets the bits specified by the compile-time mask. */
    template<auto Mask> requires enum_with_underlying<decltype(Mask), underlying_type>
    bit_flags& set(bool state = true) noexcept {
        if (state)
            my_flags |= std::to_underlying(Mask);
        else
            my_flags &= ~std::to_underlying(Mask);
        return *this;
    }

    /** Clears the bits specified by the compile-time mask. */
    template<auto Mask> requires enum_with_underlying<decltype(Mask), underlying_type>
    bit_flags& reset() noexcept {
        return set<Mask>(false);
    }

    /** Toggles the bits specified by the compile-time mask. */
    template<auto Mask> requires enum_with_underlying<decltype(Mask), underlying_type>
    bit_flags& flip() noexcept {
        my_flags ^= std::to_underlying(Mask);
        return *this;
    }

protected:
    static auto nth_bit_mask(std::uint8_t bit_index) noexcept {
        assert(bit_index < 8 * sizeof(underlying_type) && "Bit index is out-of-bounds");

        enum class bit_mask : underlying_type {};
        return static_cast<bit_mask>(underlying_type{1} << bit_index);
    }

protected:
    StorageType my_flags = {};
};

} // namespace emu
