#pragma once

#include <string>

#include "HUD.hpp"
#include "Time.hpp"
#include "World.hpp"
#include "player_obj.hpp"
#include "properties.hpp"

class Window : public sf::RenderWindow {
private:
  sf::VideoMode vm;

public:
  // ---- GameObjects -----------
  Player CammeraTarget;
  World world;
  Time time;
  HUD hud;


  Window(const sf::VideoMode &VM, std::string title,
         sf::Uint32 style = sf::Style::Default)
      : sf::RenderWindow(VM, title, style), vm(VM), hud(time) {
    Cammera.setSize(static_cast<float>(vm.width),
                    static_cast<float>(vm.height));
    setView(Cammera);

    nightOverlay.setSize(sf::Vector2f(vm.width, vm.height));
    nightOverlay.setPosition(0.f, 0.f);

  }

  sf::Clock clock;
  sf::View Cammera;

  sf::Color currentSkyColor = settings::DayColor;
  sf::RectangleShape nightOverlay;

  
  void setPositions() {
    world.setPositions(CammeraTarget);
    hud.setPositions(vm);
  }
  
  void update(float dt) { 

    CammeraTarget.update(dt);
    Cammera.setCenter(CammeraTarget.pos);
    time.update(dt, (*this), Cammera, currentSkyColor, nightOverlay);
    world.update(dt, (*this), Cammera, CammeraTarget);
    hud.update(dt, (*this), Cammera, CammeraTarget);

    setView(Cammera);
  }

  void gameLoop() {

    setFramerateLimit(settings::FPS);
    setPositions();

    // * ----------- Cammera -----------------
    Cammera.zoom(0.8);

    while (isOpen()) {
      sf::Event event;
      while (pollEvent(event)) {
        if (event.type == sf::Event::Closed)
          close();

        else if (event.type == sf::Event::KeyPressed) {
          if (event.key.code == sf::Keyboard::Key::Escape) {
            if (CammeraTarget.has_target)
              CammeraTarget.cancel_move_to();
          }
        }

        else if (event.type == sf::Event::MouseWheelScrolled) {
          if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            if (event.mouseWheelScroll.delta > 0) {
              Cammera.zoom(0.9f);
            } else if (event.mouseWheelScroll.delta < 0) {
              Cammera.zoom(1.1f);
            }
          }
        }
      }

      sf::Time dtTime = clock.restart();
      float deltaT = dtTime.asSeconds();

      update(deltaT);
      draw(world);      

      setView(getDefaultView());
      draw(nightOverlay);
      draw(hud);

      display();
    }
  }
};
