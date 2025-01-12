#include "emu/address.h"
#include "emu/test.h"

#include <type_traits>

TEST_CASE(raw_word_address) {
    emu::raw_address<emu::address_size::word> const addr(0x1234);
    ASSERT_EQ(addr.value(),  0x1234);

    ASSERT_EQ(addr.page(),   emu::address_page_index{0x12});
    ASSERT_EQ(addr.offset(), 0x34);
}

TEST_CASE(absolute_address) {
    using address_type = emu::absolute_address;
    address_type addr(0x1234);

    TEST_SUBCASE(operator_plus_equal) {
        decltype(auto) new_addr = (addr += 0xFF);
        static_assert(std::is_same_v<decltype(new_addr), address_type&>);

        ASSERT_EQ(&new_addr, &addr);
        ASSERT_EQ(addr.value(), 0x1333);
    }

    TEST_SUBCASE(operator_plus_ai) {
        decltype(auto) new_addr = addr + 0xFF;
        static_assert(std::is_same_v<decltype(new_addr), address_type>);

        ASSERT_EQ(new_addr.value(), 0x1333);
    }

    TEST_SUBCASE(operator_plus_ia) {
        decltype(auto) new_addr = 0xFF + addr;
        static_assert(std::is_same_v<decltype(new_addr), address_type>);

        ASSERT_EQ(new_addr.value(), 0x1333);
    }

    TEST_SUBCASE(operator_minus_equal) {
        decltype(auto) new_addr = (addr -= 0xFF);
        static_assert(std::is_same_v<decltype(new_addr), address_type&>);

        ASSERT_EQ(&new_addr, &addr);
        ASSERT_EQ(addr.value(), 0x1135);
    }

    TEST_SUBCASE(operator_minus) {
        decltype(auto) new_addr = addr - 0xFF;
        static_assert(std::is_same_v<decltype(new_addr), address_type>);

        ASSERT_EQ(new_addr.value(), 0x1135);
    }
}

TEST_CASE(same_page_address) {
    using address_type = emu::same_page_address;
    address_type addr(0x1234);

    TEST_SUBCASE(operator_plus_equal) {
        decltype(auto) new_addr = (addr += 0xFF);
        static_assert(std::is_same_v<decltype(new_addr), address_type&>);

        ASSERT_EQ(&new_addr, &addr);
        ASSERT_EQ(addr.value(), 0x1233);
    }

    TEST_SUBCASE(operator_plus_ai) {
        decltype(auto) new_addr = addr + 0xFF;
        static_assert(std::is_same_v<decltype(new_addr), address_type>);

        ASSERT_EQ(new_addr.value(), 0x1233);
    }

    TEST_SUBCASE(operator_plus_ia) {
        decltype(auto) new_addr = 0xFF + addr;
        static_assert(std::is_same_v<decltype(new_addr), address_type>);

        ASSERT_EQ(new_addr.value(), 0x1233);
    }

    TEST_SUBCASE(operator_minus_equal) {
        decltype(auto) new_addr = (addr -= 0xFF);
        static_assert(std::is_same_v<decltype(new_addr), address_type&>);

        ASSERT_EQ(&new_addr, &addr);
        ASSERT_EQ(addr.value(), 0x1235);
    }

    TEST_SUBCASE(operator_minus) {
        decltype(auto) new_addr = addr - 0xFF;
        static_assert(std::is_same_v<decltype(new_addr), address_type>);

        ASSERT_EQ(new_addr.value(), 0x1235);
    }
}

TEST_CASE(fixed_page_address) {
    using address_type = emu::fixed_page_address<emu::address_page_index{0x12}>;
    address_type addr(0x34);

    ASSERT_EQ(addr.value(), 0x34);

    ASSERT_EQ(addr.page(),   emu::address_page_index{0x12});
    ASSERT_EQ(addr.offset(), 0x34);

    TEST_SUBCASE(operator_plus_equal) {
        decltype(auto) new_addr = (addr += 0xFF);
        static_assert(std::is_same_v<decltype(new_addr), address_type&>);

        ASSERT_EQ(&new_addr, &addr);
        ASSERT_EQ(addr.value(), 0x33);
        ASSERT_EQ(addr.page(),  emu::address_page_index{0x12});
    }

    TEST_SUBCASE(operator_plus_ai) {
        decltype(auto) new_addr1 = addr + 0xFF;
        static_assert(std::is_same_v<decltype(new_addr1), address_type>);

        ASSERT_EQ(new_addr1.value(), 0x33);
        ASSERT_EQ(new_addr1.page(),  emu::address_page_index{0x12});
    }

    TEST_SUBCASE(operator_plus_ia) {
        decltype(auto) new_addr = 0xFF + addr;
        static_assert(std::is_same_v<decltype(new_addr), address_type>);

        ASSERT_EQ(new_addr.value(), 0x33);
        ASSERT_EQ(new_addr.page(),  emu::address_page_index{0x12});
    }

    TEST_SUBCASE(operator_minus_equal) {
        decltype(auto) new_addr = (addr -= 0xFF);
        static_assert(std::is_same_v<decltype(new_addr), address_type&>);

        ASSERT_EQ(&new_addr, &addr);
        ASSERT_EQ(addr.value(), 0x35);
        ASSERT_EQ(addr.page(),  emu::address_page_index{0x12});
    }

    TEST_SUBCASE(operator_minus) {
        decltype(auto) new_addr = addr - 0xFF;
        static_assert(std::is_same_v<decltype(new_addr), address_type>);

        ASSERT_EQ(new_addr.value(), 0x35);
        ASSERT_EQ(new_addr.page(),  emu::address_page_index{0x12});
    }
}
