#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>

// *TODO: needs a style rework

class ProgressBar : public sf::Drawable, public sf::Transformable {
private:
  sf::RectangleShape m_background;
  sf::RectangleShape m_foreground;

  float m_maxValue;
  float m_currentValue;

  // Overriding the draw method allows SFML to render this component natively
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    states.transform *= getTransform(); // Apply the class's transform matrix
    target.draw(m_background, states);
    target.draw(m_foreground, states);
  }

public:
  sf::Color default_color;
  sf::Color warning_color = sf::Color(158, 26, 11);

  ProgressBar() : m_maxValue(100.f), m_currentValue(100.f) {}

  // Initializes the size, colors, and maximum bounds of the status bar
  void init(sf::Vector2f size, sf::Color bgCol, sf::Color fgCol,
            float maxValue) {
    m_maxValue = maxValue;
    m_currentValue = maxValue;

    // Configure background shell
    m_background.setSize(size);
    m_background.setFillColor(bgCol);

    // Configure moving inner foreground bar
    m_foreground.setSize(size);
    m_foreground.setFillColor(fgCol);
    default_color = fgCol;
  }

  // Updates the inner bar's visual scaling based on a new current value
  void updateValue(float value) {
    // Clamp the value between 0 and m_maxValue to prevent drawing backwards or
    // out of bounds
    m_currentValue = std::max(0.f, std::min(value, m_maxValue));

    // Calculate progress percentage
    float percentage = m_currentValue / m_maxValue;

    if (percentage <= 0.2)
      m_foreground.setFillColor(warning_color);
    else
      m_foreground.setFillColor(default_color);

    // Scale the width of the foreground bar dynamically
    sf::Vector2f currentSize = m_background.getSize();
    m_foreground.setSize(
        sf::Vector2f(currentSize.x * percentage, currentSize.y));
  }

  void centerOrigin() {
    sf::Vector2f size = m_background.getSize();
    setOrigin(size.x / 2.f, size.y / 2.f);
  }
};