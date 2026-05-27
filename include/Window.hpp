#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
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
#include "store.hpp"

class Window : public sf::RenderWindow {
private:
  sf::VideoMode vm;
  bool wasMousePressedLastFrame = false;

public:
  island land;
  Player player;
  Farm farm;
  Text WheatText;
  Text AppleText;
  Timer PowerHourFrequncy;
  Timer PowerHourCountDown;
  Timer DayNight;
  ProgressBar PowerHourLeftTime;
  Store store;

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
    DayNight.maxTime = 60.f * 5; // 5 min
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

    nightOverlay.setSize(sf::Vector2f(vm.width, vm.height));
    nightOverlay.setPosition(0.f, 0.f);
  }

  sf::Clock clock;
  sf::View Cammera;

  const sf::Color ClearSkyColor = sf::Color(24, 152, 184);

  sf::Color DayColor = sf::Color(0, 0, 0, 0); // 0 Alpha = Completely invisible
  sf::Color NightColor =
      sf::Color(15, 15, 35, 160); // 160 Alpha = Translucent dark midnight blue
  sf::Color currentSkyColor = DayColor;
  sf::RectangleShape nightOverlay;

  int wheat_count = 0;
  int apple_count = 0;

  bool isDay = true;

  // Changes the overlay color slowly
  void setBackGroundColor(sf::Color &current, sf::Color &to, float dt,
                          float speed = 150.f) {
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

    clear(ClearSkyColor);
  }

  void update(float dt) {

    DayNight.TimePassed += dt;

    player.update(dt);
    farm.update(dt, wheat_count, apple_count, (*this), &Cammera);
    Cammera.setCenter(player.pos);
    store.update(dt, *this, &Cammera, player);

    setView(Cammera);

    WheatText.setString("Wheat: " + std::to_string(wheat_count));
    AppleText.setString("Apple: " + std::to_string(apple_count));

    if (PowerHourFrequncy.TimePassed >= PowerHourFrequncy.maxTime) {

      // Power hour Time!
      PowerHourCountDown.TimePassed += dt;

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

    if (DayNight.TimePassed > DayNight.maxTime) {
      DayNight.TimePassed = 0.f;
      isDay = !isDay;
    }

    if (isDay) {
      setBackGroundColor(currentSkyColor, DayColor, dt);
    } else {
      setBackGroundColor(currentSkyColor, NightColor, dt);
    }

    bool isMousePressedThisFrame = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    if (store.isHovered() && isMousePressedThisFrame &&
        !wasMousePressedLastFrame) {
      store.isClicked = !store.isClicked;
    }
    wasMousePressedLastFrame = isMousePressedThisFrame;
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

    store.setPosition(land.getIndex({0, 0}));

    // * ----------- Cammera -----------------
    Cammera.zoom(0.8);

    while (isOpen()) {
      sf::Event event;
      while (pollEvent(event)) {
        if (event.type == sf::Event::Closed)
          close();

        else if (event.type == sf::Event::KeyPressed) {
          if (event.key.code == sf::Keyboard::Key::Escape) {
            if (player.has_target)
              player.cancel_move_to();
            else {
              //// pass
            }
          }
        }

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
      draw(land);

      for (auto &decoration : decorations)
        draw(*decoration);

      draw(farm);
      for (const auto &battery : bateries) {
        draw(*battery);
        draw(battery->powerBar);
      }
      draw(store);

      setView(getDefaultView()); // FIXED: Switches viewport to pixel-perfect
      draw(nightOverlay);
      draw(WheatText);
      draw(AppleText);
      draw(PowerHourLeftTime);

      display();
    }
  }
};
