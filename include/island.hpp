#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>
#include <iostream>
#include <new>
#include <ostream>
#include <vector>

#include "Sprite.hpp"
#include "properties.hpp"

class island : public sf::Drawable {
private:
  // std::vector<std::unique_ptr<sf::Drawable>> children;

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {

    for (auto &sprtArray : tiles) {
      for (const Sprite &s : sprtArray) {
        target.draw(s, states);
      }
    }

    // for (auto &s : children) {
    //   target.draw(*s, states);
    // }
  }

public:
  Sprite sprt;
  int tile_size = 10;

  constexpr static int islandSize[2] = {20, 10};

  std::array<std::array<Sprite, islandSize[1]>, islandSize[0]> tiles;
  std::vector<sf::Vector2f> taken; //! Vecotr where it can be array!

  sf::Vector2f size;

  island() {
    // init & load

    for (int x = 0; x < tiles.size(); ++x) {
      for (int y = 0; y < tiles[x].size(); ++y) {
        Sprite &s = tiles[x][y];

        s.from_path_to_txt("../Assets/Tilesets/Grass.png");

        if (x == 0 && y == 0) {
          s.setTextureRect(
              sf::IntRect(settings::TILE_SIZE * 0, settings::TILE_SIZE * 0,
                          settings::TILE_SIZE, settings::TILE_SIZE));
        } else if (x == tiles.size() - 1 && y == 0) {
          s.setTextureRect(
              sf::IntRect(settings::TILE_SIZE * 2, settings::TILE_SIZE * 0,
                          settings::TILE_SIZE, settings::TILE_SIZE));
        } else if (x == tiles.size() - 1 && y == tiles[x].size() - 1) {
          s.setTextureRect(
              sf::IntRect(settings::TILE_SIZE * 2, settings::TILE_SIZE * 2,
                          settings::TILE_SIZE, settings::TILE_SIZE));
        } else if (x == 0 && y == tiles[x].size() - 1) {
          s.setTextureRect(
              sf::IntRect(settings::TILE_SIZE * 0, settings::TILE_SIZE * 2,
                          settings::TILE_SIZE, settings::TILE_SIZE));
        } else if (x > 0 && y == 0) {
          s.setTextureRect(
              sf::IntRect(settings::TILE_SIZE * 1, settings::TILE_SIZE * 0,
                          settings::TILE_SIZE, settings::TILE_SIZE));
        } else if (x == 0 && y > 0) {
          s.setTextureRect(
              sf::IntRect(settings::TILE_SIZE * 0, settings::TILE_SIZE * 1,
                          settings::TILE_SIZE, settings::TILE_SIZE));

        } else if (x == tiles.size() - 1 && y > 0) {
          s.setTextureRect(
              sf::IntRect(settings::TILE_SIZE * 2, settings::TILE_SIZE * 1,
                          settings::TILE_SIZE, settings::TILE_SIZE));
        } else if (x > 0 && y == tiles[x].size() - 1) {
          s.setTextureRect(
              sf::IntRect(settings::TILE_SIZE * 1, settings::TILE_SIZE * 2,
                          settings::TILE_SIZE, settings::TILE_SIZE));
        } else {
          s.setTextureRect(
              sf::IntRect(settings::TILE_SIZE * 1, settings::TILE_SIZE * 1,
                          settings::TILE_SIZE, settings::TILE_SIZE));
        }

        s.setScale({settings::SCALE, settings::SCALE});
        s.setPosition({(float)x * settings::DISPLAY_SCALE,
                       (float)y * settings::DISPLAY_SCALE});
      }
    }

    size.x = settings::DISPLAY_SCALE * tiles.size();
    size.y = settings::DISPLAY_SCALE * tiles[0].size();

    taken.reserve(islandSize[0] * islandSize[1]);
  }

  void setPosition(sf::Vector2f pos) {
    for (int x = 0; x < tiles.size(); ++x) {
      for (int y = 0; y < tiles[x].size(); ++y) {
        Sprite &s = tiles[x][y];

        s.setScale({settings::SCALE, settings::SCALE});
        float pixel_x = pos.x + ((float)x * settings::DISPLAY_SCALE);
        float pixel_y = pos.y + ((float)y * settings::DISPLAY_SCALE);

        s.setPosition({pixel_x, pixel_y});
      }
    }
  }

  sf::Vector2f getIndex(sf::Vector2i grid_i, bool markAsTaken = true) {

    if (grid_i.x < 0 || grid_i.x >= tiles.size()) {
      std::cerr << "Invalid position on x axis: " << grid_i.x << std::endl;
    }
    if (grid_i.y < 0 || grid_i.y >= tiles[0].size()) {
      std::cerr << "Invalid position on y axis: " << grid_i.y << std::endl;
    }

    if (markAsTaken) {
      for (sf::Vector2f &s : taken) {
        if (s == tiles[grid_i.x][grid_i.y].getPosition()) {
          std::cerr << "cannot allocate memory in taken position" << std::endl;
          throw std::bad_alloc();
        }
      }
    }

    taken.push_back(tiles[grid_i.x][grid_i.y].getPosition());

    return tiles[grid_i.x][grid_i.y].getPosition();
  }

  template <typename iterable> void markAsTaken(iterable &List) {

    for (Sprite &l : List) {
      for (sf::Vector2f &s : taken) {
        if (l.getPosition() == s) {
          std::cerr << "cannot allocate memory in taken position" << std::endl;
          throw std::bad_alloc();
        }
      }

      taken.push_back(l.getPosition());

    }

  }
};