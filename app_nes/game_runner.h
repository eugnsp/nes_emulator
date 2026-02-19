#pragma once

#include "emu/file/nes_file.h"

#include <filesystem>
#include <utility>

namespace emu::app {

class game_runner {
public:
    game_runner(std::filesystem::path const&);

    void run();

private:
    template<class Mapper>
    void run_with_mapper(std::type_identity<Mapper>);

private:
    nes_file my_nes_file;
};

} // namespace emu::app
