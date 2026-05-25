#pragma once

#include <random>

struct settings {
    inline static const int FPS = 60;
    inline static const int TILE_SIZE = 16;
    inline static const int SCALE = 3;
    inline static const int DISPLAY_SCALE = SCALE * TILE_SIZE;
    inline static const float PLANT_RATIO = 0.50;
};

template <typename T>
T genRand(T min, T max) {
    // 1. Create a static random engine so it seeds once and stays alive across function calls
    static std::random_device rd;
    static std::mt19937 generator(rd());

    // 2. Choose the correct distribution type at compile time
    if constexpr (std::is_integral_v<T>) {
        std::uniform_int_distribution<T> distribution(min, max);
        return distribution(generator);
    } else {
        std::uniform_real_distribution<T> distribution(min, max);
        return distribution(generator);
    }
}

struct Timer {
    float TimePassed = 0.f;
    float maxTime;
};