#include "emu/test/test_case.h"

#include "emu/test/output.h"
#include "emu/test/test_subcase.h"
#include "emu/test/types.h"

#include <map>
#include <print>
#include <source_location>
#include <string_view>

namespace test {

int num_assertions = 0;
int num_assertions_passed = 0;

namespace {

struct test_case {
    test_fn_ptr fn;
    std::source_location loc;
};

enum class test_case_run_result {
    passed, failed, exception
};

/** A set of all registered test cases. */
std::map<std::string_view, test_case> all_test_cases;

} // anonymous namespace

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void register_test_case(char const* id, test_fn_ptr fn, std::source_location loc) {
    all_test_cases.try_emplace(id, test_case{.fn = fn, .loc = loc});
}

test_case_run_result run_test(test_case const& tc) noexcept {
    while (true) {
        test_subcase_base::reset_on_test_case_reenter();

        try {
            tc.fn();
            return test_case_run_result::passed;
        }
        catch (const reenter_test_case&) {
            continue;
        }
        catch (const assertion_violation&) {
            return test_case_run_result::failed;
        }
        catch (const std::exception& ex) {
            std::println("{:yellow}", colorize("Exception was thrown: {}", ex.what()));
            return test_case_run_result::exception;
        }
        catch (...) {
            std::println("{:yellow}", colorize("Unknown exception was thrown"));
            return test_case_run_result::exception;
        }
    }
}

bool run_all_test_cases() {
    std::size_t const num_test_cases = all_test_cases.size();

    std::println("Running {} test case(s)...", num_test_cases);
    std::println();

    std::size_t num_test_cases_failed = 0;
    std::size_t num_exceptions = 0;

    for (auto const& [id, tc] : all_test_cases) {
        std::println("Test case: {:cyan} ({:magenta})",
            colorize("{}", id),
            colorize("{}:{}", tc.loc.file_name(), tc.loc.line())
        );

        switch (run_test(tc)) {
            case test_case_run_result::passed:
                std::println("{:green}", colorize("[V] TEST PASSED!"));
                break;

            case test_case_run_result::exception:
                ++num_exceptions;
                [[fallthrough]];

            case test_case_run_result::failed:
                std::println("{:red}", colorize("[X] TEST FAILED!"));
                ++num_test_cases_failed;
        }

        std::println();
    }

    std::println();
    std::println("Test cases: {:green} passed, {:red} failed",
        colorize("{}", num_test_cases - num_test_cases_failed),
        colorize("{}", num_test_cases_failed)
    );
    std::println("Assertions: {:green} passed, {:red} failed",
        colorize("{}", num_assertions_passed),
        colorize("{}", num_assertions - num_assertions_passed)
    );
    std::println("Exceptions: {:red}",
        colorize("{}", num_exceptions)
    );

    std::println();
    if (num_test_cases_failed == 0) {
        std::println("{:green}", colorize("ALL TESTS PASSED"));
        return true;
    }
    else {
        std::println("{:red}", colorize("THERE WERE FAILED TESTS"));
        return false;
    }
}

} // namespace test
