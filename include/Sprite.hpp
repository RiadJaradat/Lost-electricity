#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Sprite : public sf::Sprite {
public:
  sf::Texture texture;

  void from_path_to_txt(const std::string &path) { texture.loadFromFile(path); setTexture(texture); }
  void set_texture(const sf::Texture &txt) { texture = txt; setTexture(texture); }

  Sprite() {};
  Sprite(const std::string &path) { from_path_to_txt(path); };
  Sprite(const sf::Texture &txt) { set_texture(txt); };
  Sprite(const sf::Sprite &sprite, sf::Texture &txt) {};
};