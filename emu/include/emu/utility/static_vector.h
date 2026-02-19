#pragma once

#include "emu/utility/type_traits.h"

#include <cassert>
#include <cstddef>
#include <memory>
#include <new>

namespace emu {

/**
 * A dynamically-resizable array with contiguous inplace storage.
 */
template<class T, std::size_t Capacity>
class static_vector {
    static_assert(Capacity > 0, "Capacity must be greater than zero");

public:
    using size_type  = smallest_size_type<Capacity>;
    using value_type = T;

public:
    /** Constructs an empty vector. */
    static_vector() noexcept = default;

    /** Copy constructor. */
    static_vector(static_vector const& other) {
        insert(other.begin(), other.end());
    }

    /** Copy assignment. */
    static_vector& operator=(static_vector const& other) {
        if (&other != this) {
            clear();
            insert(other.begin(), other.end());
        }
        return *this;
    }

    /** Destructs a vector. */
    ~static_vector() noexcept {
        destroy_all();
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Element access

    /** Returns a reference to the element at specified location. */
    value_type& operator[](size_type pos) noexcept {
        assert(pos < my_size && "Index is out-of-bounds");
        return begin()[pos];
    }

    /** Returns a reference to the element at specified location. */
    value_type const& operator[](size_type pos) const noexcept {
        assert(pos < my_size && "Index is out-of-bounds");
        return begin()[pos];
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Iterators

    /** Returns an iterator to the first element. */
    value_type* begin() noexcept {
        return std::launder(ptr(0));
    }

    /** Returns an iterator to the first element. */
    value_type const* begin() const noexcept {
        return std::launder(ptr(0));
    }

    /** Returns an iterator past the last element. */
    value_type* end() noexcept {
        return begin() + my_size;
    }

    /** Returns an iterator past the last element. */
    value_type const* end() const noexcept {
        return begin() + my_size;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Size and capacity

    /** Returns the number of elements. */
    size_type size() const noexcept {
        return my_size;
    }

    /** Returns the capacity. */
    static constexpr size_type capacity() noexcept {
        return static_cast<size_type>(Capacity);
    }

    /** Checks whether the container is empty (its size is zero). */
    bool is_empty() const noexcept {
        return my_size == 0;
    }

    /** Checks whether the container is full (its size equals the capacity). */
    bool is_full() const noexcept {
        return my_size == capacity();
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifiers

    /** Appends the given element to the end of the container and returns a reference to the inserted element. */
    value_type& push_back(value_type const& value) {
        assert(!is_full() && "Vector is full");

        value_type* const p = std::construct_at(ptr(my_size), value);
        ++my_size; // Must be incremented only after successful construction

        return *p;
    }

    /** Erases all elements from the container. */
    void clear() noexcept {
        destroy_all();
        my_size = 0;
    }

private:
    value_type* ptr(size_type pos) noexcept {
        return reinterpret_cast<value_type*>(&my_buffer[pos]);
    }

    value_type const* ptr(size_type pos) const noexcept {
        return reinterpret_cast<value_type const*>(&my_buffer[pos]);
    }

    void insert(value_type const* first, value_type const* last) {
        while (first != last)
            push_back(*first++);
    }

    void destroy_all() noexcept {
        size_type pos = my_size;
        while (pos > 0)
            std::destroy_at(std::launder(ptr(--pos)));
    }

private:
    size_type my_size = 0;
    alignas(value_type) std::uint8_t my_buffer[Capacity][sizeof(value_type)];
};

} // namespace emu
