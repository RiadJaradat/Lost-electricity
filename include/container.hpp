#pragma once

#include "Text.hpp"
#include "battery.hpp"
#include "ui_base.hpp"

class container : public UIBase {
public:
  Battery &battery;
  Text Capacity;
  Text degradation;

  container(Battery &btry) : battery(btry) {}

  void setPosition(sf::Vector2f n_pos) {
    UIBase::setPosition(n_pos);
    rect.setPosition(n_pos);

    Capacity.setPos({
         (rect.getPosition().x - Capacity.getGlobalBounds().width) / 2,
         (rect.getPosition().y - Capacity.getGlobalBounds().height) / 2
    });
    
    degradation.setPos({
        (rect.getPosition().x - degradation.getGlobalBounds().width) / 2,
        (rect.getPosition().y - degradation.getGlobalBounds().height) / 2
    });

  }

  //* kind note: you have to update the values manually
  // *TODO make this not the case
  void update(sf::RenderWindow &window) override {
    Capacity.setString("Capacity: " + std::to_string(battery.Capacity));
    degradation.setString("Degradation: " +
                          std::to_string(battery.maxCapacity));
  }
};