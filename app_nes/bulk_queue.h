#pragma once

#include <concepts>
#include <mutex>
#include <vector>

namespace emu::app {

/** Thread-safe queue that accumulates values and allows the caller to process all queued values at once. */
template<class T>
class bulk_queue {
public:
    /** Pushes a value to the queue. */
    void push(T const& value) {
        std::scoped_lock lock(my_mutex);
        my_values.push_back(value);
    }

    /**
     * Invokes the given callable for each queued value and then clears the queue.
     * The callable is executed while holding the internal mutex.
     */
    template<std::invocable<T const&> Fn>
    void consume_all(Fn fn) {
        std::scoped_lock lock(my_mutex);
        for (T const& value : my_values)
            fn(value);
        my_values.clear();
    }

private:
    std::vector<T> my_values;
    std::mutex     my_mutex;
};

} // namespace emu::app
