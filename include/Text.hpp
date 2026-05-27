#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Text : public sf::Drawable {
private:
  sf::Font m_font;
  sf::Text m_text;

  // Allows the object to be drawn directly: window.draw(myText);
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw(m_text, states);
  }

public:
  Text() {
    // Load your font once during initialization
    if (!m_font.loadFromFile("/usr/share/fonts/dejavu-sans-fonts/DejaVuSans-Bold.ttf")) {
      std::cerr << "Text Class Error: Could not load font file!" << std::endl;
    }

    m_text.setFont(m_font);
    m_text.setCharacterSize(24); // Default readable size
    m_text.setFillColor(sf::Color::White);
  }

  // Set the string value directly
  void setString(const std::string &str) { m_text.setString(str); }

  // Set position using individual x and y coordinates
  void setPos(float x, float y) { m_text.setPosition(x, y); }

  // Set position using an SFML vector
  void setPos(sf::Vector2f pos) { m_text.setPosition(pos); }

  // Change the size of the font dynamically if needed
  void setSize(unsigned int size) { m_text.setCharacterSize(size); }

  // Change text color easily
  void setColor(sf::Color color) { m_text.setFillColor(color); }

  void setOrigin(float x, float y) { m_text.setOrigin(x, y); }

  // ADD THIS: Exposes the inner text rectangle dimensions safely
  sf::FloatRect getLocalBounds() const { return m_text.getLocalBounds(); }
  sf::FloatRect getGlobalBounds() const { return m_text.getGlobalBounds(); }
};