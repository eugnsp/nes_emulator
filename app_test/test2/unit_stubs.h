#pragma once

#include <cstdint>

/** PPU stub. */
class vppu_stub {
public:
    std::uint8_t load(auto&&) {
        return 0x00;
    }

    void store(auto&&...) {}

    void store_oam_data_dma(auto&&) noexcept {}
};

/** APU stub. */
class vapu_stub {
public:
    std::uint8_t load(auto&&) {
        return 0x00;
    }

    void store(auto&&...) {}
};
