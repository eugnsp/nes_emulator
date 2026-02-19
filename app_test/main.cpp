#include "test1.h"
#include "test2.h"

#include <cstdlib>
#include <exception>
#include <print>

int main() {
    try {
        std::println("[1/2] Running 6502 functional test...");
        emu::test::run_6502_functional_test();
        std::println("PASSED");

        std::println("[2/2] Running NES test...");
        emu::test::run_nes_cpu_test();
        std::println("PASSED");

        return EXIT_SUCCESS;
    }
    catch (std::exception const& ex) {
        std::println("FAILED: {}", ex.what());
    }
    catch (...) {
        std::println("FAILED: Unknown exception");
    }

    return EXIT_FAILURE;
}
