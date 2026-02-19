#pragma once

#include "emu/mapper/mapper_number.h"
#include "emu/utility/type_traits.h"

#include <type_traits>

namespace emu::mapper {

/** Provides compile-time information about a specific mapper class. */
template<class Mapper> struct mapper_traits;

/** Defines a type trait for a mapper class. */
#define EMU_DEFINE_MAPPER_TRAIT(Type, Number, Name)                 \
template<>                                                          \
struct mapper_traits<class Type> {                                  \
    static constexpr auto number = mapper_number{Number};           \
    static constexpr char const* name = Name;                       \
};

EMU_DEFINE_MAPPER_TRAIT(mapper_000_nrom,  0, "NROM")
EMU_DEFINE_MAPPER_TRAIT(mapper_001_mmc1,  1, "MMC1")
EMU_DEFINE_MAPPER_TRAIT(mapper_002_uxrom, 2, "UxROM")

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Compile-time list of all mapper classes. */
using all_mappers = type_list<mapper_000_nrom, mapper_001_mmc1, mapper_002_uxrom>;

/**
 * If the mapper with the given number is defined, invokes a function with an argument of type
 * "std::type_identity<Mapper>" and returns true; otherwise does nothing and returns false.
 */
template<class Fn>
bool with_mapper_type(mapper_number mapper_num, Fn&& fn) {
    auto const run = [&]<class Mapper>(std::type_identity<Mapper> mapper_type) {
        if (mapper_traits<Mapper>::number != mapper_num)
            return false;

        fn(mapper_type);
        return true;
    };

    return [&]<class... Mappers>(type_list<Mappers...>) {
        return (run(std::type_identity<Mappers>{}) || ...);
    } (all_mappers{});
}

} // namespace emu::map
