#pragma once

#include "Button.hpp"
#include "UI.hpp"
#include "clickable_sprite.hpp"
#include "player_obj.hpp"
#include "properties.hpp"
#include <vector>

class Store : public Clickable {
private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw(static_cast<Sprite>(*this), states);

    if (isClicked)
      target.draw(store_ui, states);
  }

public:
  std::vector<Sprite *> children;
  UI_element store_ui;
  Button batteryBuyBtn;

  Store() {
    from_path_to_txt("../Assets/Objects/Chest.png");
    setTextureRect(sf::IntRect(settings::TILE_SIZE * 1, settings::TILE_SIZE * 1,
                               settings::TILE_SIZE, settings::TILE_SIZE));

    setScale(settings::SCALE, settings::SCALE);

    store_ui.setRect();
    store_ui.setScale(settings::SCALE * 2, settings::SCALE * 3);
    store_ui.setPosition(getPosition() +
                         sf::Vector2f(25, -store_ui.getGlobalBounds().height));
    batteryBuyBtn.setPosition({(store_ui.getGlobalBounds().width -
                                batteryBuyBtn.getGlobalBounds().width) /
                                   2,
                               (store_ui.getGlobalBounds().height -
                                batteryBuyBtn.getGlobalBounds().height) /
                                   2});
    batteryBuyBtn.buttonText.setString("Buy new battery #50");
    batteryBuyBtn.buttonText.setSize(10);
    batteryBuyBtn.setScale(settings::SCALE, settings::SCALE);

    store_ui.children.push_back(&batteryBuyBtn);
  }

  void update(float dt, sf::RenderWindow &window, sf::View *view,
              Player &player) {

    Clickable::update(dt, window, view);
    batteryBuyBtn.update(dt, window, view);

    if (isClicked) {
      player.move_to(getPosition());
    }
  }
};
