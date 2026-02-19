#include "game_runner.h"

#include <cstdlib>
#include <exception>
#include <filesystem>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: app_nes <NES-file>" << std::endl;
        return EXIT_SUCCESS;
    }

    try {
        std::filesystem::path const nes_file_path(argv[1]);

        emu::app::game_runner runner(nes_file_path);
        runner.run();

        return EXIT_SUCCESS;
    }
    catch (std::exception const& ex) {
        std::cout << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}
