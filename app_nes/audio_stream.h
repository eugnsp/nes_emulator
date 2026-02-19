#pragma once

#include "emu/utility/spsc_ring_buffer.h"

#include <SFML/Audio.hpp>

#include <iostream>

namespace emu::app {

class audio_stream : public sf::SoundStream {
public:
    audio_stream(unsigned int sample_rate);

public:
    void push(sf::Int16 value);

protected:
    virtual bool onGetData(sf::SoundStream::Chunk& data) override;

    virtual void onSeek(sf::Time) override;

//private:
public:
    static constexpr std::size_t buffer_size = 4096;

    std::vector<sf::Int16> my_samples;
    emu::spsc_ring_buffer<sf::Int16> my_fifo;
};

} // namespace emu::app
