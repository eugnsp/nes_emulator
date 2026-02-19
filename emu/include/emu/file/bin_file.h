#pragma once

#include <cstddef>
#include <filesystem>
#include <vector>

namespace emu {

/** Reads a binary file and returns its contents. */
std::vector<std::uint8_t> read_bin_file(std::filesystem::path const&);

} // namespace emu
