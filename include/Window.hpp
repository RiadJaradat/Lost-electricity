#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Bar.hpp"
#include "Sprite.hpp"
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
  Text WheatText;
  Text AppleText;
  Timer PowerHourFrequncy;
  Timer PowerHourCountDown;
  ProgressBar PowerHourLeftTime;
  std::vector<std::unique_ptr<Battery>> bateries;
  std::vector<std::unique_ptr<Sprite>> decorations;

  float flowerToBladeRatio = 0.15;

  Window(const sf::VideoMode &VM, std::string title,
         sf::Uint32 style = sf::Style::Default)
      : sf::RenderWindow(VM, title, style), vm(VM), farm(bateries) {
    Cammera.setSize(static_cast<float>(vm.width),
                    static_cast<float>(vm.height));
    setView(Cammera);
    PowerHourFrequncy.maxTime = 60.f * 12; // 12 min
    PowerHourCountDown.maxTime = 15.f;
    PowerHourLeftTime.init(sf::Vector2f(50.f, 8.f), sf::Color(50, 50, 50),
                           sf::Color(50, 168, 82), PowerHourFrequncy.maxTime);
    PowerHourLeftTime.centerOrigin();
    PowerHourLeftTime.setScale(3 * settings::SCALE, settings::SCALE);
    PowerHourLeftTime.warning_color = sf::Color(29, 102, 171);
    PowerHourLeftTime.default_color = sf::Color(191, 161, 52);

    bateries.push_back(std::make_unique<Battery>());
    bateries.push_back(std::make_unique<Battery>());

    size_t decorationCount =
        (size_t)((land.tiles.size() * land.tiles[0].size()) *
                 settings::PLANT_RATIO);

    decorations.reserve(decorationCount);

    for (int i = 0; i < decorationCount; i++) {

      auto s = std::make_unique<Sprite>();

      float rand = genRand<float>(0, 1);

      if (rand > flowerToBladeRatio) {

        s->from_path_to_txt("../Assets/Objects/Basic Grass Biom things 1.png");
        s->setTextureRect(
            sf::IntRect(settings::TILE_SIZE * 5, settings::TILE_SIZE * 1,
                        settings::TILE_SIZE, settings::TILE_SIZE));
        s->setScale(settings::SCALE, settings::SCALE);
      } else {
        s->from_path_to_txt("../Assets/Objects/Basic Grass Biom things 1.png");
        s->setTextureRect(
            sf::IntRect(settings::TILE_SIZE * 7, settings::TILE_SIZE * 2,
                        settings::TILE_SIZE, settings::TILE_SIZE));

        s->setScale(settings::SCALE, settings::SCALE);
      }

      decorations.push_back(std::move(s));
    }
  }

  sf::Clock clock;
  sf::View Cammera;

  int wheat_count = 0;
  int apple_count = 0;

  void update(float dt) {

    player.update(dt);
    farm.update(dt, wheat_count, apple_count, (*this), &Cammera);

    Cammera.setCenter(player.pos);
    setView(Cammera);

    WheatText.setString("Wheat: " + std::to_string(wheat_count));
    AppleText.setString("Apple: " + std::to_string(apple_count));

    if (PowerHourFrequncy.TimePassed >= PowerHourFrequncy.maxTime) {

      // Power hour Time!
      PowerHourCountDown.TimePassed += dt;

      // *TODO: chang to for (auto b : batteries) { ... }

      for (const auto &battery : bateries) {
        battery->Copasity += 10.f;

        if (battery->Copasity >= battery->maxCapacity) {
          battery->Copasity = battery->maxCapacity;
        }
      }

      float timeRemaining =
          PowerHourCountDown.maxTime - PowerHourCountDown.TimePassed;

      PowerHourLeftTime.updateValue(
          (timeRemaining / PowerHourCountDown.maxTime) *
              PowerHourFrequncy.maxTime,
          (*this), &getDefaultView());

      if (PowerHourCountDown.TimePassed > PowerHourCountDown.maxTime) {
        PowerHourFrequncy.TimePassed = 0.f;
        PowerHourCountDown.TimePassed = 0.f;
      }
    } else {
      PowerHourFrequncy.TimePassed += dt;

      // FIXED: Pass the unmoving HUD layout view context here too
      PowerHourLeftTime.updateValue(PowerHourFrequncy.maxTime -
                                        PowerHourFrequncy.TimePassed,
                                    (*this), &getDefaultView());
    }
  }

  void gameLoop() {

    setFramerateLimit(settings::FPS);

    // * ----------- Positions ---------------

    player.setPosition({land.size.x / 2.f, land.size.y / 2.f});
    farm.setPosition(
        land.getIndex({(int)std::round(land.tiles.size() - farm.size.x - 1),
                       (int)std::round(land.tiles[0].size() - farm.size.y - 1)},
                      false));
    for (auto &s : farm.tiles) {
      land.markAsTaken(s);
    }

    for (size_t i = 0; i < bateries.size(); ++i) {
      bateries[i]->setPosition(land.getIndex({1, static_cast<int>(5 + i + 1)}));
    }
    PowerHourLeftTime.setPosition({(float)vm.width / 2, (float)vm.height - 20});

    WheatText.setPos({10, 10});
    WheatText.setSize(12 * settings::SCALE);
    AppleText.setPos({10, 40});
    AppleText.setSize(12 * settings::SCALE);

    for (std::unique_ptr<Sprite> &s : decorations) {

      sf::Vector2i randomPos = {genRand<int>(0, land.tiles.size() - 1),
                                genRand<int>(0, land.tiles[0].size() - 1)};

      s->setPosition(land.getIndex(randomPos, false));
    }

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

      for (auto &decoration : decorations)
        draw(*decoration);

      draw(farm);
      for (const auto &battery : bateries) {
        draw(*battery);
        draw(battery->powerBar);
      }
      setView(getDefaultView()); // FIXED: Switches viewport to pixel-perfect
                                 // screen coords
      draw(WheatText);
      draw(AppleText);
      draw(PowerHourLeftTime);

      display();
    }
  }
};