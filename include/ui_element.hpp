#pragma once

#include "Button.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>


#include "AssetsManeger.hpp"
#include "properties.hpp"

class ui_element : public sf::Drawable, public sf::Transformable {

private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw(sprt, states);
    for (auto &d : children)
      target.draw(d, states);
    target.draw(rect, states);
  }

public:
  constexpr inline static int RESERVED_CHILDREN = 5;

  sf::RectangleShape rect;
  sf::Sprite sprt;
  sf::Vector2f size;
  sf::Color FillColor;
  std::vector<Button> children;
  ui_element() {
    children.reserve(RESERVED_CHILDREN);
    setFillColor(sf::Color::Transparent);
    sprt.setTexture(Assets::UiTILe);
  }

  void setSize(sf::Vector2f n_size) {
    size = n_size;
    rect.setSize(size);
  }

  void setFillColor(sf::Color n_color) {
    FillColor = n_color;
    rect.setFillColor(FillColor);
  }

  void setRect() {
    sprt.setTextureRect(sf::IntRect(
        settings::TILE_SIZE, 0, settings::TILE_SIZE * 2, settings::TILE_SIZE));
  }

  void setSquare() {
    sprt.setTextureRect(sf::IntRect(settings::TILE_SIZE, 0, settings::TILE_SIZE,
                                    settings::TILE_SIZE));
  }

  void update(sf::RenderWindow &window) {
    rect.setPosition(getPosition());
    sprt.setPosition(rect.getPosition());
    for (auto &d : children) {
      d.update(window);
    }
  }
};