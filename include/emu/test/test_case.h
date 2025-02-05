#pragma once

#include "emu/test/types.h"

#include <cstdlib>
#include <source_location>
#include <utility>

#define TEST_DEFINE_MAIN                                                                    \
int main() {                                                                                \
    if (test::run_all_test_cases())                                                         \
        return EXIT_SUCCESS;                                                                \
    else                                                                                    \
        return EXIT_FAILURE;                                                                \
}

#define TEST_CASE(id)                                                                       \
    static void test_case_fn_##id();                                                        \
    int const test_case_reg_##id = (                                                        \
        test::register_test_case(#id, test_case_fn_##id, std::source_location::current()),  \
        0                                                                                   \
    );                                                                                      \
                                                                                            \
    static void test_case_fn_##id()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace test {

/** Registers a test case in the global array of test cases. */
void register_test_case(char const*, test_fn_ptr, std::source_location);

/** Runs all test cases, returns true iff all test cases passed. */
bool run_all_test_cases();

} // namespace test
