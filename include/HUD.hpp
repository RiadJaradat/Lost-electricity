#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <memory>
#include <stdexcept>

#include "Bar.hpp"
#include "Button.hpp"
#include "Text.hpp"
#include "Time.hpp"
#include "World.hpp"
#include "battery.hpp"
#include "player_obj.hpp"
#include "properties.hpp"
#include "ui_element.hpp"

class HUD : public sf::Drawable {
private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw(WheatText, states);
    target.draw(AppleText, states);
    target.draw(PowerHourLeftTime, states);
    target.draw(StoreBtn, states);
    if (drawUI)
      target.draw(store_ui, states);

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
  Button StoreBtn;
  ui_element store_ui;
  Button BuyBtn;

  ProgressBar PowerHourLeftTime;

  Time &time;

  bool drawUI = false;

  float fps;

  HUD(Time &t, Player &player, World &world) : time(t) {
    PowerHourLeftTime.init(sf::Vector2f(50.f, 8.f), sf::Color(50, 50, 50),
                           sf::Color(50, 168, 82),
                           time.PowerHourFrequency.maxTime);
    PowerHourLeftTime.centerOrigin();
    PowerHourLeftTime.setScale(3 * settings::SCALE, settings::SCALE);
    PowerHourLeftTime.warning_color = sf::Color(29, 102, 171);
    PowerHourLeftTime.default_color = sf::Color(191, 161, 52);
    StoreBtn.setFillColor(sf::Color(201, 134, 40));
    StoreBtn.m_text.setString("Store?");
    StoreBtn.setSize({StoreBtn.m_text.getGlobalBounds().width + 10, 100});
    store_ui.setSize({300, 150});
    store_ui.setRect();
    store_ui.sprt.setScale(
        {store_ui.rect.getSize().x / store_ui.sprt.getGlobalBounds().width,
         store_ui.rect.getSize().y / store_ui.sprt.getGlobalBounds().height});
    store_ui.setSize({300, 150});
    BuyBtn.setFillColor(sf::Color(201, 134, 40));
    BuyBtn.m_text.setString("Buy?");
    BuyBtn.setSize({BuyBtn.m_text.getGlobalBounds().width + 10, 50});
    StoreBtn.onClick = [&]() { drawUI = !drawUI; };
    BuyBtn.onClick = [&]() {
      if (player.wheat_count - prices::battery < 0) {
        throw std::runtime_error("hahaha too poor");
      }

      player.wheat_count -= prices::battery;
      world.batteries.push_back(std::make_unique<Battery>());
      world.updateBatteryPos();
    };
  }

  void setPositions(sf::VideoMode &vm) {
    WheatText.setPos({10, 10});
    WheatText.setSize(12 * settings::SCALE);
    AppleText.setPos({10, 40});
    AppleText.setSize(12 * settings::SCALE);
    PowerHourLeftTime.setPosition({(float)vm.width / 2, (float)vm.height - 20});
    StoreBtn.setPosition(10, 100);
    store_ui.setPosition(StoreBtn.getPosition() + sf::Vector2f(150, 10));
    BuyBtn.setPosition(store_ui.getPosition() + sf::Vector2f(20.f, 20.f));
    store_ui.children.push_back(BuyBtn);
  }

  void update(float dt, sf::RenderWindow &window, sf::View &view,
              Player &player) {
    StoreBtn.update(window);
    store_ui.update(window);
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

    fps = (1.f / dt);
  }
};