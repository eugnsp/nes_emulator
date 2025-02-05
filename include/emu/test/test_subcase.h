#pragma once

#define TEST_SUBCASE(id)                                                                    \
    if (test::test_subcase<struct test_subcase_tag_##id> tsc(#id); tsc.should_run())

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace test {

class test_subcase_base {
public:
    test_subcase_base(char const* id, bool& should_run);
    ~test_subcase_base();

    bool should_run() const noexcept;

    static void reset_on_test_case_reenter() noexcept;

private:
    inline static bool my_any_subcase_was_run = false;
    bool& my_should_run;
};

template<class test_subcase_tag>
class test_subcase : public test_subcase_base {
public:
    test_subcase(char const* id) : test_subcase_base(id, my_should_run)
    {}

private:
    inline static bool my_should_run = true;
};

} // namespace test
