#pragma once

#include "emu/address/linear_address.h"
#include "emu/types.h"

#include <utility>

namespace emu {

template<address_page_index Page>
class fixed_page_address : public linear_address<address_size::byte, fixed_page_address<Page>> {
private:
    using my_base = linear_address<address_size::byte, fixed_page_address>;
    friend my_base;

public:
    using my_base::my_base;

    static constexpr address_page_index page() noexcept {
        return Page;
    }

private:
    void advance(ptrdiff_t dist) noexcept {
        this->my_addr += dist;
    }
};

} // namespace emu
