#pragma once

#include "AssetsManeger.hpp"
#include "UI.hpp"
#include "clickable_sprite.hpp"
#include "player_obj.hpp"
#include "properties.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Store : public Clickable {
private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw((sf::Sprite)(*this), states);
    if (isClicked)
      target.draw(store_ui, states);
  }

public:
  ui_element store_ui; 

  Store() {
    setTexture(Assets::BoxTile);
    setTextureRect(sf::IntRect(settings::TILE_SIZE, settings::TILE_SIZE,
                               settings::TILE_SIZE, settings::TILE_SIZE));
    setScale(settings::SCALE, settings::SCALE);
  }
  void update(float dt, sf::RenderWindow &window, sf::View *view,
              Player &player) {
    Clickable::update(dt, window, view);

    if (isClicked) {
      player.move_to(getPosition());
    }
  }
};
