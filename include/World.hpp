#pragma once

#include <SFML/Graphics.hpp>

#include "farm.hpp"
#include "island.hpp"
#include "player_obj.hpp"

class World : public sf::Drawable {
private:
  float flowerToBladeRatio = 0.15;
  bool wasMousePressedLastFrame = false;

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw(land, states);
    for (auto &decoration : decorations)
      target.draw(*decoration, states);
    target.draw(farm, states);
    for (const auto &battery : batteries) {
      target.draw(*battery, states);
      // *TODO: make them for the UI.hpp
      target.draw(battery->powerBar, states);
    }
  }

public:
  island land;
  Farm farm;
  std::vector<std::unique_ptr<Battery>> batteries;
  std::vector<std::unique_ptr<sf::Sprite>> decorations;

  World() : farm(batteries) {

    batteries.push_back(std::make_unique<Battery>());
    batteries.push_back(std::make_unique<Battery>());

    size_t decorationCount =
        (size_t)((land.tiles.size() * land.tiles[0].size()) *
                 settings::PLANT_RATIO);

    decorations.reserve(decorationCount);

    for (int i = 0; i < decorationCount; i++) {

      auto s = std::make_unique<sf::Sprite>();

      float rand = genRand<float>(0, 1);

      s->setTexture(Assets::GrassBiomTile);

      if (rand > flowerToBladeRatio) {
        s->setTextureRect(
            sf::IntRect(settings::TILE_SIZE * 5, settings::TILE_SIZE * 1,
                        settings::TILE_SIZE, settings::TILE_SIZE));
        s->setScale(settings::SCALE, settings::SCALE);
      } else {
        s->setTextureRect(
            sf::IntRect(settings::TILE_SIZE * 7, settings::TILE_SIZE * 2,
                        settings::TILE_SIZE, settings::TILE_SIZE));

        s->setScale(settings::SCALE, settings::SCALE);
      }

      decorations.push_back(std::move(s));
    }
  }

  void setPositions(Player &player) {
    player.setPosition({land.size.x / 2.f, land.size.y / 2.f});
    farm.setPosition(
        land.getIndex({(int)std::round(land.tiles.size() - farm.size.x - 1),
                       (int)std::round(land.tiles[0].size() - farm.size.y - 1)},
                      false));
    for (auto &s : farm.tiles) {
      land.markAsTaken(s);
    }

    for (size_t i = 0; i < batteries.size(); ++i) {
      batteries[i]->setPosition(land.getIndex({1, static_cast<int>(5 + i + 1)}));
    }

    for (std::unique_ptr<sf::Sprite> &s : decorations) {

      sf::Vector2i randomPos = {genRand<int>(0, land.tiles.size() - 1),
                                genRand<int>(0, land.tiles[0].size() - 1)};

      s->setPosition(land.getIndex(randomPos, false));
    }
  }

  void update(float dt, sf::RenderWindow &window, sf::View &Camera,
              Player &player) {
    farm.update(dt, player.wheat_count, player.apple_count, window, &Camera);

    for (const auto &battery : batteries) {
      battery->Capacity += 10.f;

      if (battery->Capacity >= battery->maxCapacity) {
        battery->Capacity = battery->maxCapacity;
      }
    }

    for (auto &b : batteries)
      b->update(dt);
  }
};