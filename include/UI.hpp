#pragma once

#include "Sprite.hpp"
#include "properties.hpp"

#include <vector>

class ui_element : public Sprite {
private:
  std::vector<sf::Drawable *> children;
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw((Sprite)(*this), states);
    for (auto &d : children)
      target.draw(*d, states);
  }

public:
  ui_element() {
    from_path_to_txt(ASSETS "/UI/UI.png");
    setTextureRect(sf::IntRect(settings::TILE_SIZE, 0, settings::TILE_SIZE * 2,
                               settings::TILE_SIZE));
    setScale(settings::SCALE, settings::SCALE);
  }
  void add_chiled(sf::Drawable &d) { children.push_back(&d); }
};
