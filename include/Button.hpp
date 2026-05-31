#pragma once

#include "ui_base.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <optional>
#include <stdexcept>

class Button : public UIBase {
private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw(rect, states);
    target.draw(m_text, states);
  }

public:
  sf::Vector2f size;
  sf::Text m_text;
  sf::Font m_font;
  sf::Color FillColor;
  std::optional<sf::Color> hoverColor;

  std::function<void()> onClick;

  bool isClickedLastFrame = false;

  Button() {
    if (!m_font.loadFromFile(
            "/usr/share/fonts/dejavu-sans-fonts/DejaVuSans-Bold.ttf")) {
      throw std::runtime_error("unable to load font!");
    }
    m_text.setFont(m_font);

    auto bounds = m_text.getLocalBounds();

    m_text.setPosition(
        getPosition().x + (size.x - bounds.width) / 2.f - bounds.left,
        getPosition().y + (size.y - bounds.height) / 2.f - bounds.top);
  }

  void setSize(sf::Vector2f n_size) {
    size = n_size;
    rect.setSize(size);
  }

  void setFillColor(sf::Color n_color) {
    FillColor = n_color;

    if (!hoverColor) {
      hoverColor = sf::Color(std::max(0, static_cast<int>(n_color.r) - 50),
                             std::max(0, static_cast<int>(n_color.g) - 50),
                             std::max(0, static_cast<int>(n_color.b) - 50));
    }

    rect.setFillColor(FillColor);
  }

  void setHoverColor(sf::Color n_color) { hoverColor = n_color; }

  void update(sf::RenderWindow &window) override {
    rect.setPosition(getPosition());
    auto bounds = m_text.getLocalBounds();

    m_text.setPosition(
        getPosition().x + (size.x - bounds.width) / 2.f - bounds.left,
        getPosition().y + (size.y - bounds.height) / 2.f - bounds.top);

    sf::Vector2f MousePos =
        window.mapPixelToCoords(sf::Mouse::getPosition(window));

    bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    if (rect.getGlobalBounds().contains(MousePos)) {

      rect.setFillColor(*hoverColor);

      if (mousePressed && !isClickedLastFrame) {
        if (onClick)
          onClick();
      }
    } else {
      rect.setFillColor(FillColor);
    }

    isClickedLastFrame = mousePressed;
  }
};