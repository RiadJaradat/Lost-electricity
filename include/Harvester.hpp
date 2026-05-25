#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>
#include <cstddef>
#include <memory>
#include <vector>

#include "Bar.hpp"
#include "Sprite.hpp"
#include "battery.hpp"
#include "plant.hpp"
#include "properties.hpp"

class Harvester : public Sprite {
public:
  Timer onPlant;
  Timer onCharge;
  std::vector<std::unique_ptr<Battery>> &batteries;

  sf::Vector2f offset = {30, 20.f};
  sf::Vector2i farm_i = {0, 0};

  float power = 50.f;
  float maxPower = 50.f;
  float speed = 150.f;

  int direction_x = 0;
  int direction_y = 0;

  ProgressBar powerBar;

  Harvester(std::vector<std::unique_ptr<Battery>> &btries) : batteries(btries) {
    from_path_to_txt("../Assets/Characters/Tools.png");

    setTextureRect(sf::IntRect(settings::TILE_SIZE * 0, settings::TILE_SIZE * 0,
                               settings::TILE_SIZE, settings::TILE_SIZE));

    setScale(settings::SCALE, settings::SCALE);

    onPlant.maxTime = 2.f;
    onCharge.maxTime = 5.f;

    powerBar.init(sf::Vector2f(50.f, 8.f), sf::Color(50, 50, 50),
                  sf::Color(50, 168, 82), maxPower);
  }

  bool fly_to(sf::Vector2f pos, float dt) {
    sf::Vector2f current = getPosition();

    // 1. Calculate the vector pointing directly to target destination
    sf::Vector2f direction = pos - current;

    // 2. Calculate the direct Euclidean distance to target
    float distance =
        std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // 3. Arrived! If close enough, snap to target directly to eliminate
    // overshooting vibration
    if (distance <= speed * dt) {
      setPosition(pos);
      return true;
    }

    // 4. Normalize the travel direction vector and step forward
    direction /= distance;
    current.x += direction.x * speed * dt;
    current.y += direction.y * speed * dt;

    setPosition(current);
    return false; // Still en route
  }

  template <size_t X, size_t Y>
  void update(float dt, std::array<std::array<Plant, X>, Y> &plants,
              int &wheat_count, int &apple_count, sf::Vector2f farmPosition, sf::RenderWindow &window, sf::View *v) {

    if (power <= 0) {

      Battery *battery = nullptr;
      int heightest_cop = -1.f;

      for (size_t i = 0; i < batteries.size(); ++i) {
        if (batteries[i]->Copasity > heightest_cop) {
          heightest_cop = batteries[i]->Copasity;
          battery =
              batteries[i].get(); // Store the real object's memory address
        }
      }

      if (battery != nullptr) {
        if (fly_to(battery->getPosition() - farmPosition + offset, dt)) {

          onCharge.TimePassed += dt;

          if (onCharge.TimePassed > onCharge.maxTime) {
            float incomingEnergy = 0.f;
            battery->take(incomingEnergy, window, v);

            // Safely apply and clamp the energy transfer
            power += incomingEnergy;
            if (power > maxPower) {
              power = maxPower; // Keeps the harvester strictly at 10.f max
            }
            onCharge.TimePassed = 0.f;
          }
        }
      } 

      return;
    }

    power -= dt;
    powerBar.updateValue(power, window, v);

    if (fly_to(plants[farm_i.x][farm_i.y].getPosition() + offset, dt)) {

      onPlant.TimePassed += dt;

      if (onPlant.TimePassed >= onPlant.maxTime) {
        onPlant.TimePassed = 0.f;

        Plant &p = plants[farm_i.x][farm_i.y];

        fly_to(p.getPosition() + offset, dt);

        if (!p.isWatered) {
          p.isWatered = true;
          p.watered.TimePassed = 0;
        }
        if (p.isReady) {
          p.state = Ripeness::SEED;
          p.isReady = false;
          p.isWatered = true;
          p.growth.TimePassed = 0.f;
          p.watered.TimePassed = 0.f;
          if (p.isApple)
            apple_count++;
          else
            wheat_count++;

          p.setTextureRect(sf::IntRect(settings::TILE_SIZE * (int)p.state,
                                       settings::TILE_SIZE * (int)p.isApple,
                                       settings::TILE_SIZE,
                                       settings::TILE_SIZE));
        }

        if (farm_i.x < X - 1) {
          farm_i.x++;
        } else if (farm_i.y < Y - 1) {
          farm_i.y++;
          farm_i.x = 0;
        } else {
          farm_i.x = 0;
          farm_i.y = 0;
        }
      }
    } else {
      onPlant.TimePassed = 0.f;
    }
  }

  void setPosition(sf::Vector2f pos) {
    Sprite::setPosition(pos);

    // Position the bar slightly above the battery texture (e.g., 15 pixels up)
    powerBar.setPosition(pos.x, pos.y - 15.f);
  }
};