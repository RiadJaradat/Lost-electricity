#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>
#include <vector>

#include "AssetsManeger.hpp"
#include "properties.hpp"
#include "ui_base.hpp"

class ui_element : public sf::Sprite {

private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw((sf::Sprite)*this, states);
    for (auto &d : children)
      target.draw(d, states);
    target.draw(rect, states);
  }

public:
  constexpr inline static int RESERVED_CHILDREN = 5;

  sf::RectangleShape rect;
  sf::Vector2f size;
  sf::Color FillColor;
  std::vector<UIBase> children;
  ui_element() {
    children.reserve(RESERVED_CHILDREN);
    setFillColor(sf::Color::Transparent);
    setTexture(Assets::UiTILe);
  }

  void setSize(sf::Vector2f n_size) {
    size = n_size;
    rect.setSize(size);
  }

  void setFillColor(sf::Color n_color) {
    FillColor = n_color;
    rect.setFillColor(FillColor);
  }

  void setRect(bool isVertical = false) {
    if (!isVertical) {
      setTextureRect(sf::IntRect(settings::TILE_SIZE, 0,
                                      settings::TILE_SIZE * 2,
                                      settings::TILE_SIZE));
    } else {
    }
  }

  void setSquare() {
    setTextureRect(sf::IntRect(settings::TILE_SIZE, 0, settings::TILE_SIZE,
                                    settings::TILE_SIZE));
  }

  void update(sf::RenderWindow &window) {
    rect.setPosition(getPosition());
    setPosition(rect.getPosition());
    for (auto &d : children) {
      d.update(window);
    }
  }
};
