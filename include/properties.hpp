#pragma once

#include <SFML/Graphics.hpp>
#include <random>

struct settings {
  inline static const int FPS = 60;
  inline static const int TILE_SIZE = 16;
  inline static const int SCALE = 3;
  inline static const int DISPLAY_SCALE = SCALE * TILE_SIZE;
  inline static const float PLANT_RATIO = 0.50;
  inline static const sf::Color DayColor = sf::Color(0, 0, 0, 0);
  inline static const sf::Color NightColor = sf::Color(15, 15, 35, 160);
  inline static const sf::Color ClearSkyColor = sf::Color(24, 152, 184);
};

template <typename T> T genRand(T min, T max) {
  // 1. Create a static random engine so it seeds once and stays alive across
  // function calls
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

// Changes the overlay color slowly
inline void setBackGroundColor(sf::Color &current, const sf::Color &to, float dt,
                               sf::RectangleShape &nightOverlay,
                               sf::RenderWindow &window, float speed = 150.f) {
  auto stepChannel = [](sf::Uint8 &curr, sf::Uint8 target, float delta,
                        float spd) {
    if (curr < target)
      curr = std::min(static_cast<int>(target),
                      static_cast<int>(curr + spd * delta));
    else if (curr > target)
      curr = std::max(static_cast<int>(target),
                      static_cast<int>(curr - spd * delta));
  };

  stepChannel(current.r, to.r, dt, speed);
  stepChannel(current.g, to.g, dt, speed);
  stepChannel(current.b, to.b, dt, speed);
  stepChannel(current.a, to.a, dt, speed); // Steps transparency smoothly

  nightOverlay.setFillColor(current);

  window.clear(settings::ClearSkyColor);
}

enum prices {
  battery = 50
};