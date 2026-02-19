#include "audio_stream.h"

namespace emu::app {

audio_stream::audio_stream(unsigned int sample_rate) : my_fifo(buffer_size) {
    my_samples.resize(512);
    initialize(1, sample_rate);
}

void audio_stream::push(sf::Int16 value) {
    if (!my_fifo.push(value))
        std::cout << "buffer is full" << std::endl;
}

bool audio_stream::onGetData(sf::SoundStream::Chunk& data) {
    std::size_t const chunk_size = my_fifo.pop(my_samples);
    if (chunk_size < my_samples.size()) {
        std::cout << "Chunk size: " << chunk_size << std::endl;
        if (chunk_size > 0)
            std::fill(my_samples.begin() + chunk_size, my_samples.end(), my_samples[chunk_size - 1]);
        else
            std::fill(my_samples.begin() + chunk_size, my_samples.end(), 0);
    }

    data.samples = my_samples.data();
    data.sampleCount = my_samples.size();
    return true;
}

void audio_stream::onSeek(sf::Time) {
}

} // namespace emu::app
