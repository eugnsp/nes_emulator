#pragma once

#include "emu/utility/type_traits.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <memory>
#include <ranges>
#include <stdexcept>

namespace emu {

/**
 * Fix-sized dynamically allocated array.
 */
template<class T, std::size_t Size>
class dynamic_array {
public:
    using size_type  = smallest_size_type<Size>;
    using value_type = T;

    using iterator       = value_type*;
    using const_iterator = value_type const*;

public:
    /** Constructs the array with all elements default-initialized. */
    dynamic_array() = default;

    /**
    * Constructs the array from a sized range (first default-initializes elements, then copies).
    * Throws if the range exceeds the array size, and value-initializes any remaining elements.
    */
    template<std::ranges::sized_range Range> requires std::indirectly_copyable<std::ranges::iterator_t<Range>, iterator>
    explicit dynamic_array(Range&& range) {
        if (std::ranges::size(range) > Size)
            throw std::invalid_argument("dynamic_array: Input range exceeds array size");

        iterator const tail = std::ranges::copy(range, begin()).out;
        std::ranges::fill(tail, end(), value_type());
    }

    dynamic_array(dynamic_array const& other) {
        std::ranges::copy(other, begin());
    }

    dynamic_array& operator=(dynamic_array const& other) {
        std::ranges::copy(other, begin());
        return *this;
    }

    dynamic_array(dynamic_array&&) noexcept = default;
    dynamic_array& operator=(dynamic_array&&) noexcept = default;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Element access

    /** Returns a reference to the element at specified location. */
    value_type& operator[](size_type const pos) noexcept {
        assert(pos < size() && "Array index is out-of-bounds");
        return my_data[pos];
    }

    /** Returns a reference to the element at specified location. */
    value_type const& operator[](size_type const pos) const noexcept {
        assert(pos < size() && "Array index is out-of-bounds");
        return my_data[pos];
    }

    /** Returns a pointer to the underlying array serving as element storage. */
    value_type* data() noexcept {
        return my_data.get();
    }

    /** Returns a pointer to the underlying array serving as element storage. */
    value_type const* data() const noexcept {
        return my_data.get();
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Iterators

    /** Returns an iterator to the first element. */
    iterator begin() noexcept {
        return data();
    }

    /** Returns an iterator to the first element. */
    const_iterator begin() const noexcept {
        return data();
    }

    /** Returns an iterator past the last element. */
    iterator end() noexcept {
        return data() + Size;
    }

    /** Returns an iterator past the last element. */
    const_iterator end() const noexcept {
        return data() + Size;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Size

    /** Returns the number of elements. */
    static constexpr size_type size() noexcept {
        return static_cast<size_type>(Size);
    }

private:
    std::unique_ptr<value_type[]> my_data = std::make_unique_for_overwrite<value_type[]>(Size);
};

} // namespace emu
