#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <array>
#include <iostream>
#include <vector>

#include "AssetsManeger.hpp"
#include "Harvester.hpp"
#include "battery.hpp"
#include "plant.hpp"
#include "properties.hpp"

class Farm : public sf::Drawable, public sf::Transformable {
private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    states.transform *= getTransform();

    for (const auto &sprtArray : tiles) {
      for (const sf::Sprite &s : sprtArray) {
        target.draw(s, states);
      }
    }

    for (const auto &plantArray : plants) {
      for (const Plant &p : plantArray) {
        target.draw(p, states);
      }
    }

    target.draw(harvester, states);
    target.draw(harvester.powerBar, states);
  }

public:
  constexpr static int FarmSize[2] = {5, 5};

  std::array<std::array<sf::Sprite, FarmSize[1]>, FarmSize[0]> tiles;
  std::array<std::array<Plant, FarmSize[1] - 2>, FarmSize[0] - 2> plants;

  sf::Vector2i size;

  Harvester harvester;

  Farm(std::vector<std::unique_ptr<Battery>> &batteries)
      : harvester(batteries) {

    for (int x = 0; x < tiles.size(); ++x) {
      for (int y = 0; y < tiles[x].size(); ++y) {
        sf::Sprite &s = tiles[x][y];

        s.setTexture(Assets::DirtTile);

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

    for (int x = 0; x < plants.size(); ++x) {
      for (int y = 0; y < plants[x].size(); ++y) {

        Plant &p = plants[x][y];
        sf::Vector2f tileLocalPos = tiles[x + 1][y + 1].getPosition();
        p.setPosition(tileLocalPos);
      }
    }

    size.x = tiles.size();
    size.y = tiles[0].size();
  }

  void setPosition(sf::Vector2f pos) { sf::Transformable::setPosition(pos); }

  sf::Vector2f getIndex(sf::Vector2i grid_i) {

    if (grid_i.x < 0 || grid_i.x >= tiles.size()) {
      std::cerr << "Invalid position on x axis: " << grid_i.x << std::endl;
    } else if (grid_i.x != tiles.size() - 1) {
      grid_i.x++;
    }
    if (grid_i.y < 0 || grid_i.y >= tiles[0].size()) {
      std::cerr << "Invalid position on y axis: " << grid_i.y << std::endl;
    } else if (grid_i.y != tiles[0].size() - 1) {
      grid_i.y++;
    }

    return tiles[grid_i.x][grid_i.y].getPosition();
  }

  void update(float dt, int &wheat_count, int &apple_count,
              sf::RenderWindow &window, sf::View *v) {
    for (auto &plantArray : plants) {
      for (Plant &p : plantArray) {
        p.update(dt);
      }
    }

    harvester.update(dt, plants, wheat_count, apple_count, getPosition(),
                     window, v);
  }
};