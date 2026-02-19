#pragma once

#include <bit>
#include <concepts>
#include <cstdint>
#include <type_traits>
#include <utility>

namespace emu {

/** Concept that models enum types whose underlying type is unsigned. */
template<class Enum>
concept unsigned_enum = std::is_enum_v<Enum> && std::is_unsigned_v<std::underlying_type_t<Enum>>;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Retrieves the low-order byte from the given 16-bit value. */
inline constexpr std::uint8_t get_lo_byte(std::uint16_t word) noexcept {
    return static_cast<std::uint8_t>(word & 0xFF);
}

/** Retrieves the high-order byte from the given 16-bit value. */
inline constexpr std::uint8_t get_hi_byte(std::uint16_t word) noexcept {
    return static_cast<std::uint8_t>(word >> 8);
}

/** Constructs a 16-bit value from the given low- and high-order bytes. */
inline constexpr std::uint16_t to_word(std::uint8_t byte_lo, std::uint8_t byte_hi) noexcept {
    return static_cast<std::uint16_t>((byte_hi << 8) | byte_lo);
}

/** Returns true iff the least significant bit (bit 0) is set. */
inline constexpr bool is_lsb_set(std::uint16_t word) noexcept {
    return (word & 0x01) != 0;
}

/** Returns true iff the sign bit (bit 7) of the given 8-bit value is set. */
inline constexpr bool is_sign_bit_set(std::uint8_t byte) noexcept {
    return (byte & 0x80) != 0;
}

/** Returns true iff the carry-out bit (bit 8) is set. */
inline constexpr bool is_carry_out_bit_set(std::uint16_t word) noexcept {
    return (word & 0x0100) != 0;
}

/** Left 9-bit rotation: [b7 b6 b5 b4 b3 b2 b1 b0], bit_lo -> b7, [b6 b5 b4 b3 b2 b1 b0 bit_lo]. */
inline constexpr std::pair<bool, std::uint8_t> rotate9_left(std::uint8_t byte, bool bit_lo) noexcept {
    std::uint16_t const word = static_cast<std::uint16_t>((byte << 1) | bit_lo);
    return {is_carry_out_bit_set(word), get_lo_byte(word)};
}

/** Right 9-bit rotation: bit_hi, [b7 b6 b5 b4 b3 b2 b1 b0] -> [bit_hi b7 b6 b5 b4 b3 b2 b1], b0. */
inline constexpr std::pair<std::uint8_t, bool> rotate9_right(bool bit_hi, std::uint8_t byte) noexcept {
    std::uint16_t const word = static_cast<std::uint16_t>((bit_hi ? 0x100 : 0) | byte);
    return {get_lo_byte(word >> 1), is_lsb_set(word)};
}

/** Left 9-bit shift: [b7 b6 b5 b4 b3 b2 b1 b0] -> b7, [b6 b5 b4 b3 b2 b1 b0 0]. */
inline constexpr std::pair<bool, std::uint8_t> shift9_left(std::uint8_t byte) noexcept {
    return rotate9_left(byte, 0);
}

/** Right 9-bit shift: [b7 b6 b5 b4 b3 b2 b1 b0] -> [0 b7 b6 b5 b4 b3 b2 b1], b0. */
inline constexpr std::pair<std::uint8_t, bool> shift9_right(std::uint8_t byte) noexcept {
    return rotate9_right(0, byte);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Checks whether an enum value has exactly one bit set. */
template<unsigned_enum Mask>
constexpr bool has_single_bit(Mask mask) noexcept {
    return std::has_single_bit(std::to_underlying(mask));
}

/** Checks whether an enum value has a single non-empty contiguous block of set bits. */
template<unsigned_enum Mask>
constexpr bool has_contiguous_bits(Mask mask) noexcept {
    auto const n = std::to_underlying(mask);
    return n > 0 && (n & (n + (n & -n))) == 0;
}

/** Bitwise AND of enum values. All arguments must be the same enum type. */
template<unsigned_enum Enum, std::same_as<Enum>... Enums>
constexpr Enum bitwise_and(Enum e, Enums... es) noexcept {
    return static_cast<Enum>(
        (std::to_underlying(e) & ... & std::to_underlying(es))
    );
}

/** Bitwise OR of enum values. All arguments must be the same enum type. */
template<unsigned_enum Enum, std::same_as<Enum>... Enums>
constexpr Enum bitwise_or(Enum e, Enums... es) noexcept {
    return static_cast<Enum>(
        (std::to_underlying(e) | ... | std::to_underlying(es))
    );
}

} // namespace emu
