#include "emu/mapper/mapper_number.h"

#include "emu/mapper/mapper_traits.h"

#include <format>
#include <string>
#include <utility>

namespace emu::mapper {

/** Returns a human-readable mapper name. */
std::string to_string(mapper_number mapper_num) {
    char const* mapper_name = "Unknown";
    auto const assign_mapper_name = [&]<class Mapper>(std::type_identity<Mapper>) {
        mapper_name = mapper_traits<Mapper>::name;
    };

    with_mapper_type(mapper_num, assign_mapper_name);
    return std::format("{} ({:03d})", mapper_name, std::to_underlying(mapper_num));
}

} // namespace emu::map
