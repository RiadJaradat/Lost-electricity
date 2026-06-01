#pragma once

#include "Button.hpp"
#include "ui_base.hpp"
#include "ui_element.hpp"

class Manage : public UIBase {
private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw(ManageBtn, states);
    if (drawUI)
      target.draw(Manage_ui, states);
  }

public:
  Button ManageBtn;
  ui_element Manage_ui;

  bool drawUI = false;

  Manage() {
    ManageBtn.setFillColor(sf::Color(201, 134, 40));
    ManageBtn.m_text.setString("Manage?");
    ManageBtn.setSize({ManageBtn.m_text.getGlobalBounds().width + 10, 100});
    Manage_ui.setSize({400, 600});

    Manage_ui.setTextureRect(
        sf::IntRect(settings::TILE_SIZE * 0, settings::TILE_SIZE * 1,
                    settings::TILE_SIZE, settings::TILE_SIZE * 2));
    Manage_ui.setScale(
        {Manage_ui.rect.getSize().x / Manage_ui.getGlobalBounds().width,
         Manage_ui.rect.getSize().y / Manage_ui.getGlobalBounds().height});
    ManageBtn.onClick = [&]() { drawUI = !drawUI; };
  }

  void setPositions() {
    ManageBtn.setPosition(10, 210);
    Manage_ui.setPosition(ManageBtn.getPosition() + sf::Vector2f(150, 10));
  }

  void update(sf::RenderWindow &window) override {
    ManageBtn.update(window);
    Manage_ui.update(window);
  }
};