#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "Bar.hpp"
#include "Text.hpp"
#include "Time.hpp"
#include "player_obj.hpp"
#include "properties.hpp"

class HUD : public sf::Drawable {
private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw(WheatText, states);
    target.draw(AppleText, states);
    target.draw(PowerHourLeftTime, states);
  }

public:
  Text WheatText;
  Text AppleText;

  ProgressBar PowerHourLeftTime;

  Time &time;

  HUD(Time &t) : time(t) {
    PowerHourLeftTime.init(sf::Vector2f(50.f, 8.f), sf::Color(50, 50, 50),
                           sf::Color(50, 168, 82),
                           time.PowerHourFrequency.maxTime);
    PowerHourLeftTime.centerOrigin();
    PowerHourLeftTime.setScale(3 * settings::SCALE, settings::SCALE);
    PowerHourLeftTime.warning_color = sf::Color(29, 102, 171);
    PowerHourLeftTime.default_color = sf::Color(191, 161, 52);
  }

  void setPositions(sf::VideoMode &vm) {
    WheatText.setPos({10, 10});
    WheatText.setSize(12 * settings::SCALE);
    AppleText.setPos({10, 40});
    AppleText.setSize(12 * settings::SCALE);
    PowerHourLeftTime.setPosition({(float)vm.width / 2, (float)vm.height - 20});
  }

  void update(float dt, sf::RenderWindow &window, sf::View &view,
              Player &player) {
    WheatText.setString("Wheat: " + std::to_string(player.wheat_count));
    AppleText.setString("Apple: " + std::to_string(player.apple_count));
    if (time.PowerHourFrequency.TimePassed >= time.PowerHourFrequency.maxTime) {

      // Power hour Time!
      time.PowerHourCountDown.TimePassed += dt;

      float timeRemaining =
          time.PowerHourCountDown.maxTime - time.PowerHourCountDown.TimePassed;

      PowerHourLeftTime.updateValue(
          (timeRemaining / time.PowerHourCountDown.maxTime) *
              time.PowerHourFrequency.maxTime,
          window, &window.getDefaultView());

      if (time.PowerHourCountDown.TimePassed >
          time.PowerHourCountDown.maxTime) {
        time.PowerHourFrequency.TimePassed = 0.f;
        time.PowerHourCountDown.TimePassed = 0.f;
      }
    } else {
      time.PowerHourFrequency.TimePassed += dt;

      // FIXED: Pass the unmoving HUD layout view context here too
      PowerHourLeftTime.updateValue(time.PowerHourFrequency.maxTime -
                                        time.PowerHourFrequency.TimePassed,
                                    window, &window.getDefaultView());
    }
  }
};