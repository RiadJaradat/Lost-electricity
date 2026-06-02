#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>
#include <iostream>
#include <new>
#include <ostream>
#include <vector>

#include "AssetsManeger.hpp"
#include "properties.hpp"

class island : public sf::Drawable {
private:
  // std::vector<std::unique_ptr<sf::Drawable>> children;

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {

    for (auto &sprtArray : tiles) {
      for (const sf::Sprite &s : sprtArray) {
        target.draw(s, states);
      }
    }

    // for (auto &s : children) {
    //   target.draw(*s, states);
    // }
  }

public:
  sf::Sprite sprt;
  int tile_size = 10;

  constexpr static int islandSize[2] = {20, 10};

  std::array<std::array<sf::Sprite, islandSize[1]>, islandSize[0]> tiles;
  std::vector<sf::Vector2f> taken; //! Vector where it can be array!

  sf::Vector2f size;

  island() {
    // init & load

    for (int x = 0; x < tiles.size(); ++x) {
      for (int y = 0; y < tiles[x].size(); ++y) {
        sf::Sprite &s = tiles[x][y];

        s.setTexture(Assets::GrassTile);

        int tx = 1;
        int ty = 1;

        if (x == 0)
          tx = 0;
        else if (x == tiles.size() - 1)
          tx = 2;

        if (y == 0)
          ty = 0;
        else if (y == tiles[x].size() - 1)
          ty = 2;

        s.setTextureRect(sf::IntRect(tx * settings::TILE_SIZE,
                                     ty * settings::TILE_SIZE,
                                     settings::TILE_SIZE, settings::TILE_SIZE));

        s.setScale({settings::SCALE, settings::SCALE});
        s.setPosition({(float)x * settings::DISPLAY_SCALE,
                       (float)y * settings::DISPLAY_SCALE});
      }
    }

    size.x = settings::DISPLAY_SCALE * tiles.size();
    size.y = settings::DISPLAY_SCALE * tiles[0].size();
  }

  void setPosition(sf::Vector2f pos) {
    for (int x = 0; x < tiles.size(); ++x) {
      for (int y = 0; y < tiles[x].size(); ++y) {
        sf::Sprite &s = tiles[x][y];

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

      taken.push_back(tiles[grid_i.x][grid_i.y].getPosition());
    }

    return tiles[grid_i.x][grid_i.y].getPosition();
  }

  template <typename iterable> void markAsTaken(iterable &List) {
    for (auto &l : List) {
      sf::Vector2f pos = l.getPosition();
      if (pos.x == 0.f && pos.y == 0.f)
        continue;

      bool alreadyPresent = false;
      for (const auto &s : taken) {
        if (pos == s) {
          alreadyPresent = true;
          break;
        }
      }
      if (!alreadyPresent) {
        taken.push_back(pos);
      }
    }
  }
};