#pragma once

#include "Sprite.hpp"
#include "Text.hpp"
#include "clickable_sprite.hpp"
#include "properties.hpp"
class Button : public Clickable {
private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw(static_cast<Sprite>((*this)), states);
    target.draw(buttonText);
  }

  void update_text_layout() {
    // Accessing the underlying sf::Text component (change .text to whatever
    // your inner variable is named)
    float text_x = getPosition().x + (getGlobalBounds().width / 2.f) -
                   (buttonText.getLocalBounds().width / 2.f);
    float text_y = getPosition().y + (getGlobalBounds().height / 2.f) -
                   (buttonText.getLocalBounds().height / 1.5f);

    buttonText.setPos(text_x, text_y);
  }

public:
  Text buttonText;

  Button() {

    from_path_to_txt("../Assets/UI/UI.png");
    setTextureRect(sf::IntRect(settings::TILE_SIZE * 1, settings::TILE_SIZE * 0,
                               settings::TILE_SIZE * 2, settings::TILE_SIZE));
    buttonText.setSize(16);
  }

  void setPosition(const sf::Vector2f &position) {
    Clickable::setPosition(position);
    update_text_layout();
  }

  void setPosition(float x, float y) {
    Clickable::setPosition(x, y);
    update_text_layout();
  }
};