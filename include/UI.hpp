#pragma once

#include "AssetsManeger.hpp"
#include "properties.hpp"

#include <SFML/Graphics.hpp>

class ui_element : public sf::Sprite {
private:
  std::vector<sf::Drawable *> children;
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw((sf::Sprite)(*this), states);
    for (auto &d : children)
      target.draw(*d, states);
  }

public:
  ui_element() {
    setTexture(Assets::UiTILe);
    setTextureRect(sf::IntRect(settings::TILE_SIZE, 0, settings::TILE_SIZE * 2,
                               settings::TILE_SIZE));
    setScale(settings::SCALE, settings::SCALE); 
  }
  void add_child(sf::Drawable &d) { children.push_back(&d); }
};
