#pragma once

#include "properties.hpp"

class Time {
public:
  Timer PowerHourFrequency;
  Timer PowerHourCountDown;
  Timer DayNight;

  bool isDay = true;

  Time() {
    PowerHourFrequency.maxTime = 60.f * 12; // 12 min
    PowerHourCountDown.maxTime = 15.f;
    DayNight.maxTime = 60.f * 5; // 5 min
  }

  void update(float dt, sf::RenderWindow &window, sf::View &view,
              sf::Color currentSkyColor, sf::RectangleShape &nightOverlay) {
    DayNight.TimePassed += dt;

    if (DayNight.TimePassed > DayNight.maxTime) {
      DayNight.TimePassed = 0.f;
      isDay = !isDay;
    }

    if (isDay) {
      setBackGroundColor(currentSkyColor, settings::DayColor, dt, nightOverlay,
                         window);
    } else {
      setBackGroundColor(currentSkyColor, settings::NightColor, dt,
                         nightOverlay, window);
    }
  }
};