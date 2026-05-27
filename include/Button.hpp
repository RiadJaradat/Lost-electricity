#pragma once

#include "Sprite.hpp"
#include "Text.hpp"
#include "clickable_sprite.hpp"
#include "properties.hpp"

class Button : public Clickable {
private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw((Sprite)(*this), states);
    target.draw(buttonText, states);
  }
public:
  Text buttonText;

  Button(const std::string &ButtonText) {
    from_path_to_txt(ASSETS"/Objects/Chest.png");
    setTextureRect(sf::IntRect(settings::TILE_SIZE, settings::TILE_SIZE, settings::TILE_SIZE, settings::TILE_SIZE));
    setScale(settings::SCALE, settings::SCALE);

    buttonText.setString(ButtonText);
    buttonText.setPos(getPosition() + sf::Vector2f(0, 0));
    buttonText.setSize(8);
  }
};