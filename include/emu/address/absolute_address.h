#pragma once

#include "emu/address/linear_address.h"
#include "emu/address/types.h"
#include "emu/types.h"

namespace emu {

class absolute_address : public linear_address<address_size::word, absolute_address> {
private:
    using my_base = linear_address<address_size::word, absolute_address>;
    friend my_base;

public:
    using my_base::my_base;

private:
    void advance(ptrdiff_t dist) noexcept {
        my_addr += dist;
    }
};

} // namespace emu
