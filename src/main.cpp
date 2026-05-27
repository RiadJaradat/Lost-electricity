

#include <SFML/Graphics.hpp>

#include "Window.hpp"
#include "AssetsManeger.hpp"

sf::VideoMode VM = sf::VideoMode::getDesktopMode();

int main() {
    Assets();

    Window window(VM, "Lost Electricity", sf::Style::Fullscreen);

    window.gameLoop();

    return 0;
}
