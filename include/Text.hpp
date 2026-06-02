#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <string>

class Text : public sf::Drawable {
private:
  sf::Font m_font;
  sf::Text m_text;

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw(m_text, states);
  }

public:
  Text() {
    if (!m_font.loadFromFile(
            "/usr/share/fonts/dejavu-sans-fonts/DejaVuSans-Bold.ttf"))
      std::cerr << "Text Class Error: Could not load font file!" << std::endl;
    m_text.setFont(m_font);
    m_text.setCharacterSize(24);
    m_text.setFillColor(sf::Color::White);
  }

  void setString(const std::string &str) { m_text.setString(str); }
  void setPos(float x, float y) { m_text.setPosition(x, y); }
  void setPos(sf::Vector2f pos) { m_text.setPosition(pos); }
  void setSize(unsigned int size) { m_text.setCharacterSize(size); }
  void setColor(sf::Color color) { m_text.setFillColor(color); }
  void setOrigin(float x, float y) { m_text.setOrigin(x, y); }
  void setScale(sf::Vector2f s) { m_text.setScale(s); }

  sf::Transform getTransform() const { return m_text.getTransform(); }
  sf::FloatRect getLocalBounds() const { return m_text.getLocalBounds(); }
  sf::FloatRect getGlobalBounds() const { return m_text.getGlobalBounds(); }
};