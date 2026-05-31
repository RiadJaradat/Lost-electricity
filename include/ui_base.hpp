#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

class UIBase : public sf::Sprite {
public:
  sf::RectangleShape rect;

  virtual ~UIBase() = default;
  virtual void update(sf::RenderWindow &window) {}
};