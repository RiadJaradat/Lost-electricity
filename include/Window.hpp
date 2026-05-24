#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <cmath>
#include <string>
#include <vector>

#include "Bar.hpp"
#include "Text.hpp"
#include "battery.hpp"
#include "farm.hpp"
#include "island.hpp"
#include "player_obj.hpp"
#include "properties.hpp"

class Window : public sf::RenderWindow {
private:
  sf::VideoMode vm;

public:
  island land;
  Player player;
  Farm farm;
  Battery battery;
  Text WheatText;
  Text AppleText;
  Timer PowerHourFrequncy;
  Timer PowerHourCountDown;
  ProgressBar PowerHourLeftTime;

  Window(const sf::VideoMode &VM, std::string title,
         sf::Uint32 style = sf::Style::Default)
      : sf::RenderWindow(VM, title, style), vm(VM), farm(battery) {
    Cammera.setSize(static_cast<float>(vm.width),
                    static_cast<float>(vm.height));
    setView(Cammera);
    PowerHourFrequncy.maxTime = 180.f; // 3 min
    PowerHourCountDown.maxTime = 15.f;
    PowerHourLeftTime.init(sf::Vector2f(50.f, 8.f), sf::Color(50, 50, 50),
                           sf::Color(50, 168, 82), PowerHourFrequncy.maxTime);
    PowerHourLeftTime.centerOrigin();
    PowerHourLeftTime.setScale(3 * settings::SCALE, settings::SCALE);
    PowerHourLeftTime.warning_color = sf::Color(29, 102, 171);
    PowerHourLeftTime.default_color = sf::Color(191, 161, 52);
  }

  sf::Clock clock;
  sf::View Cammera;

  int wheat_count = 0;
  int apple_count = 0;

  void update(float dt) {

    player.update(dt);
    farm.update(dt, wheat_count, apple_count);

    Cammera.setCenter(player.pos);
    setView(Cammera);

    WheatText.setString("Wheat: " + std::to_string(wheat_count));
    AppleText.setString("Apple: " + std::to_string(apple_count));

    if (PowerHourFrequncy.TimePassed >= PowerHourFrequncy.maxTime) {

      // Power hour Time!
      PowerHourCountDown.TimePassed += dt;

      // *TODO: chang to for (auto b : batteries) { ... }

      battery.Copasity += 10.f;

      if (battery.Copasity >= battery.maxCapacity) {
        battery.Copasity = battery.maxCapacity;
      }

      float timeRemaining =
          PowerHourCountDown.maxTime - PowerHourCountDown.TimePassed;

      PowerHourLeftTime.updateValue(
          (timeRemaining / PowerHourCountDown.maxTime) *
          PowerHourFrequncy.maxTime);

      if (PowerHourCountDown.TimePassed > PowerHourCountDown.maxTime) {
        PowerHourFrequncy.TimePassed = 0.f;
        PowerHourCountDown.TimePassed = 0.f;
      }
    } else {
      PowerHourFrequncy.TimePassed += dt;

      PowerHourLeftTime.updateValue(PowerHourFrequncy.maxTime -
                                    PowerHourFrequncy.TimePassed);
    }
  }

  void gameLoop() {

    setFramerateLimit(settings::FPS);

    // * ----------- Positions ---------------

    player.setPosition({land.size.x / 2.f, land.size.y / 2.f});
    farm.setPosition(land.getIndex(
        {(int)std::round(land.tiles.size() - farm.size.x - 1),
         (int)std::round(land.tiles[0].size() - farm.size.y - 1)}));
    battery.setPosition(land.getIndex({1, 5}));
    PowerHourLeftTime.setPosition({(float)vm.width / 2, (float)vm.height - 20});

    WheatText.setPos({10, 10});
    WheatText.setSize(12 * settings::SCALE);
    AppleText.setPos({10, 40});
    AppleText.setSize(12 * settings::SCALE);

    // * ----------- Cammera -----------------
    Cammera.zoom(0.8);

    while (isOpen()) {
      sf::Event event;
      while (pollEvent(event)) {
        if (event.type == sf::Event::Closed)
          close();

        else if (event.type == sf::Event::MouseWheelScrolled) {
          // Only zoom if it's the vertical mouse wheel
          if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {

            if (event.mouseWheelScroll.delta > 0) {
              // Scrolled UP -> Zoom IN (makes things look bigger/closer)
              Cammera.zoom(0.9f);
            } else if (event.mouseWheelScroll.delta < 0) {
              // Scrolled DOWN -> Zoom OUT (makes things look smaller/further
              // away)
              Cammera.zoom(1.1f);
            }
          }
        }
      }

      sf::Time dtTime = clock.restart();
      float deltaT = dtTime.asSeconds();

      update(deltaT);
      clear(sf::Color(24, 152, 184));
      draw(land);
      draw(farm);
      draw(battery);
      draw(battery.powerBar);

      setView(getDefaultView()); // FIXED: Switches viewport to pixel-perfect
                                 // screen coords
      draw(WheatText);
      draw(AppleText);
      draw(PowerHourLeftTime);

      display();
    }
  }
};