#pragma once

#include "emu/address/raw_address.h"
#include "emu/types.h"

namespace emu {

/**
 * A linear address: A CRTP base class for an address that supports address arithmetic.
 */
template<address_size Size, class DerivedAddr>
class linear_address : public raw_address<Size> {
private:
    using my_base = raw_address<Size>;

public:
    using my_base::my_base;

    DerivedAddr& operator+=(ptrdiff_t dist) noexcept {
        auto& self = static_cast<DerivedAddr&>(*this);
        self.advance(dist);
        return self;
    }

    DerivedAddr& operator-=(ptrdiff_t dist) noexcept {
        auto& self = static_cast<DerivedAddr&>(*this);
        self.advance(-dist);
        return self;
    }
};

template<address_size Size, class DerivedAddr>
DerivedAddr operator+(linear_address<Size, DerivedAddr> addr, ptrdiff_t dist) noexcept {
    addr += dist;
    return static_cast<const DerivedAddr&>(addr);
}

template<address_size Size, class DerivedAddr>
DerivedAddr operator+(ptrdiff_t dist, linear_address<Size, DerivedAddr> addr) noexcept {
    return addr + dist;
}

template<address_size Size, class DerivedAddr>
DerivedAddr operator-(linear_address<Size, DerivedAddr> addr, ptrdiff_t dist) noexcept {
    addr -= dist;
    return static_cast<const DerivedAddr&>(addr);
}

} // namespace emu
