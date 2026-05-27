#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "Sprite.hpp"
#include "properties.hpp"

enum Ripeness {
  SEED = 1, // *SEED: the plant in the beginning of its life
  TOD = 2,  // *TOD (todler): the plant in its start
  ADLT = 3, // *ADLT (adult): almost fully grown
  RIPE = 4  // *RIPE: the plant when it is fully grown
};

class Plant : public Sprite {
public:
  Ripeness state = Ripeness::SEED;

  Timer growth;
  Timer watered;

//   float TimePassed = 0.f;
//   float growthTime = 10.f;
//   float drayTime = 20.f;

  bool isApple = false;

  bool isWatered = true;
  bool isReady = false;

  Plant() {

    from_path_to_txt(ASSETS"/Objects/Basic_Plants.png");

    growth.maxTime = genRand<float>(6.f, 12.f);
    watered.maxTime = genRand<float>(18.f, 24.f);

    setTextureRect(sf::IntRect(settings::TILE_SIZE * (int)state,
                               settings::TILE_SIZE * 0, settings::TILE_SIZE,
                               settings::TILE_SIZE));
    setScale(settings::SCALE, settings::SCALE);
  }

  void update(float dt) {
    isReady = state == Ripeness::RIPE;
    if (isReady) return;
    isWatered = watered.TimePassed <= watered.maxTime;

    watered.TimePassed += dt;   
    isWatered? growth.TimePassed += dt: 0;

    if (growth.TimePassed >= growth.maxTime) {
      growth.TimePassed = 0.f;
      int next = (int)state + 1;
      state = (Ripeness)next;
      setTextureRect(sf::IntRect(settings::TILE_SIZE * (int)state,
                                 settings::TILE_SIZE * (int)isApple, settings::TILE_SIZE,
                                 settings::TILE_SIZE));
    }
  }
};

// *TODO:
// the robot will both water the plant and harvest them