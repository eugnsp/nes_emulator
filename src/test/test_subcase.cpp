#include "emu/test/test_subcase.h"

#include "emu/test/output.h"
#include "emu/test/types.h"

#include <exception>
#include <print>

namespace test {

test_subcase_base::test_subcase_base(char const* id, bool& should_run) : my_should_run(should_run) {
    if (my_any_subcase_was_run)
        throw reenter_test_case();

    if (my_should_run)
        std::println("  * {:cyan}", colorize("{}", id));
}

test_subcase_base::~test_subcase_base() {
    if (!my_should_run || std::uncaught_exceptions() > 0)
        return;

    my_should_run = false;
    my_any_subcase_was_run = true;
}

bool test_subcase_base::should_run() const noexcept {
    return my_should_run;
}

void test_subcase_base::reset_on_test_case_reenter() noexcept {
    my_any_subcase_was_run = false;
}

} // namespace test
