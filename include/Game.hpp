#pragma once

// #include "HUD.hpp"
#include <GameObjects.hpp>

class Game {
public:
  World world;
  Time WorldTime;
  Player CameraTarget;

  Game() {} // : hud(WorldTime) {}

  void BuyBattery() {
    if (CameraTarget.wheat_count - prices::battery < 0) {
      throw std::runtime_error("hahaha too poor");
    }

    CameraTarget.wheat_count -= prices::battery;
    world.batteries.push_back(std::make_unique<Battery>());
    world.updateBatteryPos();
  }
};