#pragma once

#include "Button.hpp"
#include "ui_base.hpp"
#include "ui_element.hpp"

class Store : public UIBase {
private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw(StoreBtn, states);
    if (drawUI)
      target.draw(store_ui, states);
  }

public:
  Button StoreBtn;
  ui_element store_ui;
  Button BuyBtn;

  bool drawUI = false;

  Store() {
    StoreBtn.setFillColor(sf::Color(201, 134, 40));
    StoreBtn.m_text.setString("Store?");
    StoreBtn.setSize({StoreBtn.m_text.getGlobalBounds().width + 10, 100});
    store_ui.setSize({300, 150});
    store_ui.setRect();
    store_ui.setScale(
        {store_ui.rect.getSize().x / store_ui.getGlobalBounds().width,
         store_ui.rect.getSize().y / store_ui.getGlobalBounds().height});
    BuyBtn.setFillColor(sf::Color(201, 134, 40));
    BuyBtn.m_text.setString("Buy?");
    BuyBtn.setSize({BuyBtn.m_text.getGlobalBounds().width + 10, 50});
    StoreBtn.onClick = [&]() { drawUI = !drawUI; };
    BuyBtn.onClick = [&]() {
    
    };
  }

  void setPositions() {
    StoreBtn.setPosition(10, 100);
    store_ui.setPosition(StoreBtn.getPosition() + sf::Vector2f(150, 10));
    BuyBtn.setPosition(store_ui.getPosition() + sf::Vector2f(20.f, 20.f));
    store_ui.children.push_back(&BuyBtn);
  }

  void update(sf::RenderWindow &window) override {
    StoreBtn.update(window);
    store_ui.update(window);
  }
};