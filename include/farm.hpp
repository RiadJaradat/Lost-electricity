#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>

#include "Sprite.hpp"
#include "battery.hpp"
#include "properties.hpp"
#include "plant.hpp"
#include "Harvester.hpp"

class Farm : public sf::Drawable, public sf::Transformable {
private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    states.transform *= getTransform();

    for (const auto &sprtArray : tiles) {
      for (const Sprite &s : sprtArray) {
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

  std::array<std::array<Sprite, FarmSize[1]>, FarmSize[0]> tiles;
  std::array<std::array<Plant, FarmSize[1] - 2>, FarmSize[0] - 2> plants;

  sf::Vector2i size;

  Harvester harvester;

  Farm(Battery &battery) : harvester(battery) {

    for (int x = 0; x < tiles.size(); ++x) {
      for (int y = 0; y < tiles[x].size(); ++y) {
        Sprite &s = tiles[x][y];

        s.from_path_to_txt("../Assets/Tilesets/Tilled_Dirt_v2.png");

        if (x == 0 && y == 0) {
          s.setTextureRect(
              sf::IntRect(settings::TILE_SIZE * 0, settings::TILE_SIZE * 0,
                          settings::TILE_SIZE, settings::TILE_SIZE));
        } else if (x == tiles.size() - 1 && y == tiles[x].size() - 1) {
          s.setTextureRect(
              sf::IntRect(settings::TILE_SIZE * 2, settings::TILE_SIZE * 2,
                          settings::TILE_SIZE, settings::TILE_SIZE));
        } else if (x == 0 && y == tiles[x].size() - 1) {
          s.setTextureRect(
              sf::IntRect(settings::TILE_SIZE * 0, settings::TILE_SIZE * 2,
                          settings::TILE_SIZE, settings::TILE_SIZE));
        } else if (x == tiles.size() - 1 && y > 0) {
          s.setTextureRect(
              sf::IntRect(settings::TILE_SIZE * 2, settings::TILE_SIZE * 1,
                          settings::TILE_SIZE, settings::TILE_SIZE));
        } else if (x == tiles.size() - 1 && y == 0) {
          s.setTextureRect(
              sf::IntRect(settings::TILE_SIZE * 2, settings::TILE_SIZE * 0,
                          settings::TILE_SIZE, settings::TILE_SIZE));
        } else if (y == 0 && x > 0) {
          s.setTextureRect(
              sf::IntRect(settings::TILE_SIZE * 1, settings::TILE_SIZE * 0,
                          settings::TILE_SIZE, settings::TILE_SIZE));
        } else if (x == 0 && y > 0) {
          s.setTextureRect(
              sf::IntRect(settings::TILE_SIZE * 0, settings::TILE_SIZE * 1,
                          settings::TILE_SIZE, settings::TILE_SIZE));
        } else if (y == tiles[x].size() - 1 && x > 0) {
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

  void update(float dt, int &wheat_count, int &apple_count) {
    for (auto &plantArray : plants) {
      for (Plant &p : plantArray) {
        p.update(dt);
      }
    }

    harvester.update(dt, plants, wheat_count, apple_count, getPosition());
  }

};