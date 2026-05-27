#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>

#include "Sprite.hpp"

class Clickable : public Sprite {
private:
  bool m_isHovered = false;

public:
  Clickable() : Sprite() {}

  // Pass a reference to the window, and a pointer to the specific view this
  // sprite belongs to
  void update(float dt, sf::RenderWindow &window,
              const sf::View *targetView = nullptr) {
    // 1. Get raw pixel position of the mouse relative to the window
    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);

    // 2. Convert pixel coordinates using the custom target view context
    sf::Vector2f mouseWorldPos =
        targetView ? window.mapPixelToCoords(mousePixelPos, *targetView)
                   : window.mapPixelToCoords(mousePixelPos);

    // 3. Check if the sprite's global bounding box contains the translated
    // mouse position
    if (getGlobalBounds().contains(mouseWorldPos)) {
      m_isHovered = true;
      // Optional: Add hover visual effect (e.g., tinting the sprite)
      setColor(sf::Color(200, 200, 200));
    } else {
      m_isHovered = false;
      setColor(sf::Color::White); // Reset to normal
    }
  }

  // Helper method to check the hover state externally
  bool isHovered() const { return m_isHovered; }


  bool isClicked = false;
};