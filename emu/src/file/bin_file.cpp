#include "emu/file/bin_file.h"

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <vector>

namespace emu {

std::vector<std::uint8_t> read_bin_file(std::filesystem::path const& file_path) {
    std::size_t const buff_size = std::filesystem::file_size(file_path);

    std::ifstream file;
    file.exceptions(std::ios::badbit | std::ios::failbit);
    file.open(file_path, std::ios::binary);

    std::vector<std::uint8_t> buff(buff_size);
    file.read(reinterpret_cast<char*>(buff.data()), buff_size);

    return buff;
}

} // namespace emu
