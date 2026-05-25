#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include "Bar.hpp"
#include "Sprite.hpp"
#include "properties.hpp"

class Battery : public Sprite {
public:
  float Copasity = 200.f;
  float maxCapacity = 200.f;
  float give_ammount = 50;

  ProgressBar powerBar;

  Battery() {
    from_path_to_txt("../Assets/Objects/Basic Furniture.png");

    setTextureRect(sf::IntRect(settings::TILE_SIZE * 5, settings::TILE_SIZE * 3,
                               settings::TILE_SIZE, settings::TILE_SIZE));

    setScale(settings::SCALE, settings::SCALE);

    powerBar.init(sf::Vector2f(50.f, 8.f), sf::Color(50, 50, 50),
                  sf::Color(50, 168, 82), maxCapacity);
  }

  void take(float &harvester_power, sf::RenderWindow &window, sf::View *v) {
    Copasity -= give_ammount;

    if (Copasity >= 0) {
      harvester_power += give_ammount;
      powerBar.updateValue(Copasity, window, v);
      return;
    }

    Copasity = 0.f; // Clamp to 0
    powerBar.updateValue(Copasity, window, v);
    // *TODO: make a visuall indicator that indicates that the power of the
    // battery is not enough
  }

  void setPosition(sf::Vector2f pos) {
    Sprite::setPosition(pos);

    // Position the bar slightly above the battery texture (e.g., 15 pixels up)
    powerBar.setPosition(pos.x, pos.y - 15.f);
  }

};