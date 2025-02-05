#pragma once

namespace test
{

/** Exception of this type is thrown to reenter test case to run the next subcase. */
class reenter_test_case {};

/** Exception of this type is thrown when test assertion is violated. */
class assertion_violation {};

/** Test case function type. */
using test_fn_ptr = void (*)();

} // namespace test
