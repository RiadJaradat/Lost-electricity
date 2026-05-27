#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <vector>

#include "Sprite.hpp"
#include "properties.hpp"

class UI_element : public Sprite {
private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw(static_cast<Sprite>(*this), states);

    for (auto &s : children)
      target.draw(*s, states);
  }

public:
  std::vector<sf::Drawable *> children;

  bool isApple = false;

  UI_element() { from_path_to_txt("../Assets/UI/UI.png"); }

  void setRect() {
    setTextureRect(sf::IntRect(settings::TILE_SIZE * 1,
                               settings::TILE_SIZE * (int)isApple,
                               settings::TILE_SIZE * 2, settings::TILE_SIZE));
  }
  void setSquare() {
    setTextureRect(sf::IntRect(settings::TILE_SIZE * 0,
                               settings::TILE_SIZE * (int)isApple,
                               settings::TILE_SIZE, settings::TILE_SIZE));
  }
};
