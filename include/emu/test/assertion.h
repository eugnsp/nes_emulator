#pragma once

#include "emu/test/types.h"
#include "emu/test/output.h"

#include <functional>
#include <print>
#include <source_location>

#define BINARY_ASSERT(lhs, rhs, op, predicate)                                              \
    do {                                                                                    \
        test::binary_assertion<predicate>(                                                  \
            test::arg{lhs, #lhs}, test::arg{rhs, #rhs},                                     \
            op, std::source_location::current()                                             \
        );                                                                                  \
    } while(false)

#define ASSERT_EQ(lhs, rhs) BINARY_ASSERT(lhs, rhs, "==", std::equal_to<>)
#define ASSERT_NE(lhs, rhs) BINARY_ASSERT(lhs, rhs, "!=", std::not_equal_to<>)

#define ASSERT_LT(lhs, rhs) BINARY_ASSERT(lhs, rhs, "<",  std::less<>)
#define ASSERT_LE(lhs, rhs) BINARY_ASSERT(lhs, rhs, "<=", std::less_equal<>)

#define ASSERT_GT(lhs, rhs) BINARY_ASSERT(lhs, rhs, ">",  std::greater<>)
#define ASSERT_GE(lhs, rhs) BINARY_ASSERT(lhs, rhs, ">=", std::greater_equal<>)

namespace test {

template<class T>
struct arg {
    T const& value;
    char const* value_str;
};

template<class BinaryPredicate, class L, class R>
void binary_assertion(arg<L> lhs, arg<R> rhs, char const* op, std::source_location loc) {
    extern int num_assertions;
    extern int num_assertions_passed;

    ++num_assertions;
    if (BinaryPredicate()(lhs.value, rhs.value)) {
        ++num_assertions_passed;
        return;
    }

    std::println("{:yellow} {:magenta}", colorize("Assertion failed at"), colorize("{}:{}", loc.file_name(), loc.line()));

    std::println("{:blue} with values {:blue}",
        colorize("{} {} {}", lhs.value_str, op, rhs.value_str),
        colorize("{} {} {}", to_string(lhs.value), op, to_string(rhs.value))
    );

    throw assertion_violation();
}

} // namespace test
