#pragma once

#include <SFML/Graphics.hpp>

#include "Bar.hpp"
#include "Manage.hpp"
#include "Store.hpp"
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
    target.draw(store, states);
    target.draw(manage, states);

#ifdef NDEBUG
#else
    Text FPS;
    std::string text = std::format("FPS: {:.1f}", fps);
    FPS.setString(text);
    FPS.setSize(8);
    FPS.setPos(target.getSize().x - FPS.getGlobalBounds().width - 10, 5);
    target.draw(FPS, states);
#endif
  }

public:
  Text WheatText;
  Text AppleText;
  Store store;
  Manage manage;

  ProgressBar PowerHourLeftTime;

  Time &time;

  float fps;

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
    store.setPositions();
    manage.setPositions();
  }

  void update(float dt, sf::RenderWindow &window, sf::View &view,
              Player &player) {
    store.update(window);
    manage.update(window);
    
    WheatText.setString("Wheat: " + std::to_string(player.wheat_count));
    AppleText.setString("Apple: " + std::to_string(player.apple_count));

    fps = (1.f / dt);

    bool powerHourActive =
        time.PowerHourFrequency.TimePassed >= time.PowerHourFrequency.maxTime;

    if (!powerHourActive) {
      time.PowerHourFrequency.TimePassed += dt;

      float newValue =
          time.PowerHourFrequency.maxTime - time.PowerHourFrequency.TimePassed;

      PowerHourLeftTime.updateValue(newValue, window, &window.getDefaultView());
      return;
    }

    time.PowerHourCountDown.TimePassed += dt;
    float timeRemaining =
        time.PowerHourCountDown.maxTime - time.PowerHourCountDown.TimePassed;
    float newValue = (timeRemaining / time.PowerHourCountDown.maxTime) *
                     time.PowerHourFrequency.maxTime;

    PowerHourLeftTime.updateValue(newValue, window, &window.getDefaultView());
    if (time.PowerHourCountDown.TimePassed > time.PowerHourCountDown.maxTime) {
      time.PowerHourFrequency.TimePassed = 0.f;
      time.PowerHourCountDown.TimePassed = 0.f;
    }
  }
};
