#pragma once

#include <atomic>
#include <cstddef>
#include <concepts>
#include <memory>
#include <new>
#include <span>

#include <generator>

namespace emu {

/** Single producer single consumer lock-free ring buffer class. */
template<class T> requires std::default_initializable<T>
class spsc_ring_buffer {
public:
    using value_type = T;
    using size_type  = std::size_t;

public:
    explicit spsc_ring_buffer(size_type capacity) : my_buffer_size(capacity + 1) {
        my_buffer = std::make_unique_for_overwrite<value_type[]>(my_buffer_size);
        my_read_ptr = my_write_ptr = begin();
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Size and capacity

    /**
     * Returns the current number of elements in the buffer.
     * Safe to call concurrently from producer or consumer.
     */
    size_type size() const noexcept {
        value_type const* const read_ptr  = my_read_ptr.load();
        value_type const* const write_ptr = my_write_ptr.load();

        if (write_ptr >= read_ptr)
            return static_cast<size_type>(write_ptr - read_ptr);
        else
            return my_buffer_size - static_cast<size_type>(read_ptr - write_ptr);
    }

    /** Returns the maximum number of elements the buffer can hold. */
    size_type capacity() const noexcept {
        return my_buffer_size - 1;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Modifiers

    /**
     * Pushes one element into the buffer. Returns false if the buffer is full.
     * Must be called only by the producer thread.
     */
    bool push(value_type const& value) noexcept {
        value_type* write_ptr = my_write_ptr.load(std::memory_order_relaxed);
        value_type* next_ptr = write_ptr + 1;
        if (next_ptr == end())
            next_ptr = begin();

        if (next_ptr == my_read_ptr.load(std::memory_order_acquire))
            return false;

        *write_ptr = value;
        my_write_ptr.store(next_ptr, std::memory_order_release);

        return true;
    }

    /**
     * Pops up elements into the given span and returns the number of elements actually popped.
     * Must be called only by the consumer thread.
     */
    [[nodiscard]] size_type pop(std::span<value_type> dest) noexcept {
        value_type const* read_ptr  = my_read_ptr.load(std::memory_order_relaxed);
        value_type* write_ptr = my_write_ptr.load(std::memory_order_acquire);
        if (read_ptr == write_ptr)
            return 0;

        size_type count = 0;
        while (count < dest.size() && read_ptr != write_ptr) {
            dest[count++] = *read_ptr++;
            if (read_ptr == end())
                read_ptr = begin();
        }

        my_read_ptr.store(read_ptr, std::memory_order_release);
        return count;
    }

private:
    value_type* begin() {
        return my_buffer.get();
    }

    value_type* end() {
        return my_buffer.get() + my_buffer_size;
    }

private:
    std::atomic<value_type const*> my_read_ptr;     // Read pointer, owned by the consumer

    std::unique_ptr<value_type[]> my_buffer;        // Underlying storage for elements
    size_type my_buffer_size;                       // Total buffer size (including the sentinel)

    alignas(std::hardware_destructive_interference_size)
    std::atomic<value_type*> my_write_ptr;          // Write pointer, owned by the producer

    static_assert(std::atomic<value_type*>::is_always_lock_free);
};

} // namespace emu
