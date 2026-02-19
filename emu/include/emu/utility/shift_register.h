#pragma once

#include <algorithm>
#include <array>
#include <cstddef>

namespace emu {

template<class T, std::size_t Size>
class shift_register {
public:
    T& front() {
        return my_data.front();
    }

    T const& back() const {
        return my_data.back();
    }

    void shift_left() {
        std::ranges::move_backward(my_data.begin(), my_data.end() - 1, my_data.end());
    }

private:
    std::array<T, Size> my_data;
};

} // namespace emu
