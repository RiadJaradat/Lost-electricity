#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Mouse.hpp>

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

  void init(sf::Vector2f size, sf::Color bgCol, sf::Color fgCol,
            float maxValue) {
    m_maxValue = maxValue;
    m_currentValue = maxValue;

    m_background.setSize(size);
    m_background.setFillColor(bgCol);

    m_foreground.setSize(size);
    m_foreground.setFillColor(fgCol);
    default_color = fgCol;
  }

  void updateValue(float value, sf::RenderWindow &window,
                   const sf::View *targetView) {
    m_currentValue = std::max(0.f, std::min(value, m_maxValue));
    float percentage = m_currentValue / m_maxValue;

    sf::Vector2f currentSize = m_background.getSize();
    m_foreground.setSize(
        sf::Vector2f(currentSize.x * percentage, currentSize.y));

    sf::Color baseColor = (percentage <= 0.2f) ? warning_color : default_color;

    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);

    sf::Vector2f mouseWorldPos =
        targetView ? window.mapPixelToCoords(mousePixelPos, *targetView)
                   : window.mapPixelToCoords(mousePixelPos);

    sf::FloatRect barBounds =
        getTransform().transformRect(m_background.getGlobalBounds());

    if (barBounds.contains(mouseWorldPos)) {
      m_foreground.setFillColor(sf::Color(100, 100, 100));
    } else {
      m_foreground.setFillColor(baseColor);
    }
  }

  void centerOrigin() {
    sf::Vector2f size = m_background.getSize();
    setOrigin(size.x / 2.f, size.y / 2.f);
  }
};