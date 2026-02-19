#include "audio_stream.h"

#include <algorithm>
#include <chrono>
#include <print>
#include <span>
#include <thread>

namespace emu::app {

/** Creates a mono audio stream with the given sample rate. */
audio_stream::audio_stream(unsigned int sample_rate) : my_fifo(buffer_size) {
    initialize(1, sample_rate);
}

/** Pushes a block into the internal FIFO. Blocks until enough space becomes available. */
void audio_stream::push(std::span<sf::Int16 const> values) {
    while (true) {
        if (my_fifo.push(values))
            return;

        auto const drain_seq = my_drain_seq.load(std::memory_order_acquire);

        if (my_fifo.push(values))
            return;

        my_drain_seq.wait(drain_seq, std::memory_order_relaxed);
    }
}

/** Waits until the FIFO has been prefilled enough for playback to start. */
void audio_stream::wait_until_prefilled() {
    while (my_fifo.size() < samples_size)
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

/** Requests a new chunk of audio samples from the stream source. */
bool audio_stream::onGetData(sf::SoundStream::Chunk& data) {
    auto const chunk_size = my_fifo.pop(my_samples);

    if (chunk_size < my_samples.size()) {
        std::println("Audio buffer underflow, {} elements", chunk_size);

        sf::Int16 const fill_value = (chunk_size > 0 ? my_samples[chunk_size - 1] : 0);
        std::fill(my_samples.begin() + chunk_size, my_samples.end(), fill_value);
    }

    if (chunk_size > 0) {
        my_drain_seq.fetch_add(1, std::memory_order_release);
        my_drain_seq.notify_one();
    }

    data.samples = my_samples.data();
    data.sampleCount = my_samples.size();
    return true;
}

/** Changes the current playing position in the stream source. */
void audio_stream::onSeek(sf::Time) {
    // does nothing
}

} // namespace emu::app
