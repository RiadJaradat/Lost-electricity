

#include <GameObjects.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

#include "AssetsManeger.hpp"
#include "Game.hpp"
#include "HUD.hpp"

sf::VideoMode vm = sf::VideoMode::getDesktopMode();

sf::Clock GameClock;
sf::View Camera;
sf::RenderWindow window(vm, "Lost Electricity", sf::Style::Fullscreen);
sf::Color currentSkyColor = settings::DayColor;
sf::RectangleShape nightOverlay;


Game MainGame;
HUD hud(MainGame.WorldTime, MainGame);

void setPositions() {
  MainGame.world.setPositions(MainGame.CameraTarget);
  hud.setPositions(vm);
}

void update(float dt) {

  MainGame.CameraTarget.update(dt);
  Camera.setCenter(MainGame.CameraTarget.pos);
  MainGame.WorldTime.update(dt, window, Camera, currentSkyColor, nightOverlay);
  MainGame.world.update(dt, window, Camera, MainGame.CameraTarget);
  hud.update(dt, window, Camera, MainGame.CameraTarget);

  window.setView(Camera);
}

void eventLoop(sf::Event &event) {
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed)
      window.close();

    else if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::Key::Escape)
        if (MainGame.CameraTarget.has_target)
          MainGame.CameraTarget.cancel_move_to();
    }

    else if (event.type == sf::Event::MouseWheelScrolled) {
      if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
        if (event.mouseWheelScroll.delta > 0)
          Camera.zoom(0.9f);
        else if (event.mouseWheelScroll.delta < 0)
          Camera.zoom(1.1f);
      }
    }
  }
}

int main() {

#ifdef NDEBUG
#else
  std::cout
      << "Warning:\n\tGame ran in Debug mode, which makes the game much slower "
         "compered to the release mode.\n\n* Consider switching to the release "
         "mode if playing on an official release of the game\n";
#endif

  Assets();

  Camera.setSize(static_cast<float>(vm.width), static_cast<float>(vm.height));

  window.setView(Camera);
  window.setFramerateLimit(settings::FPS);

  nightOverlay.setSize(sf::Vector2f(vm.width, vm.height));
  nightOverlay.setPosition(0.f, 0.f);

  setPositions();

  while (window.isOpen()) {
    sf::Event event;
    eventLoop(event);

    sf::Time dtTime = GameClock.restart();
    float deltaT = dtTime.asSeconds();

    window.clear();

    update(deltaT);
    window.draw(MainGame.world);

    window.setView(window.getDefaultView());
    window.draw(nightOverlay);
    window.draw(hud);

    window.display();
  }

  return 0;
}
