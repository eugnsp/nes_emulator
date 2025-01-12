#pragma once

#include "emu/address/linear_address.h"
#include "emu/types.h"
#include "emu/utility.h"

#include <utility>

namespace emu {

class same_page_address : public linear_address<address_size::word, same_page_address> {
private:
    using my_base = linear_address<address_size::word, same_page_address>;
    friend my_base;

public:
    using my_base::my_base;

private:
    void advance(ptrdiff_t dist) noexcept {
        uint8_t const new_offset = offset() + dist;
        my_addr = to_word(new_offset, std::to_underlying(page()));
    }
};

} // namespace emu
