

#include <SFML/Graphics.hpp>

#include "Window.hpp"

sf::VideoMode VM = sf::VideoMode::getDesktopMode();

int main() {

    Window window(VM, "Lost Electricity", sf::Style::Fullscreen);

    window.gameLoop();

    return 0;
}
