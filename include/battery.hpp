#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>

#include "AssetsManeger.hpp"
#include "Bar.hpp"
#include "properties.hpp"

class Battery : public sf::Sprite {
public:
  float Capacity = 200.f;
  float maxCapacity = 200.f;
  float give_amount = 50;

  ProgressBar powerBar; 
  Timer degradation;

  Battery() {
    setTexture(Assets::FurnitureTile);

    setTextureRect(sf::IntRect(settings::TILE_SIZE * 5, settings::TILE_SIZE * 3,
                               settings::TILE_SIZE, settings::TILE_SIZE));

    setScale(settings::SCALE, settings::SCALE);

    powerBar.init(sf::Vector2f(50.f, 8.f), sf::Color(50, 50, 50),
                  sf::Color(50, 168, 82), maxCapacity);

    degradation.maxTime = 60.f;
  }

  void take(float &harvester_power, sf::RenderWindow &window, sf::View *v) {
    Capacity -= give_amount;

    if (Capacity >= 0) {
      harvester_power += give_amount;
      powerBar.updateValue(Capacity, window, v);
      return;
    }

    Capacity = 0.f; // Clamp to 0
    powerBar.updateValue(Capacity, window, v);
    // *TODO: make a visual indicator that indicates that the power of the
    // battery is not enough
  }

  void setPosition(sf::Vector2f pos) {
    Sprite::setPosition(pos);

    // Position the bar slightly above the battery texture (e.g., 15 pixels up)
    powerBar.setPosition(pos.x, pos.y - 15.f);
  }

  void update(float dt) {
    degradation.TimePassed += dt;

    if (degradation.TimePassed > degradation.maxTime) {
      degradation.TimePassed = 0.f;
      maxCapacity -= 10.f;
    }

  }

};