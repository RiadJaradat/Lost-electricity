#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

#include "Sprite.hpp"

class Player : public Sprite {
private:
  sf::Vector2f target_pos;
  float arrival_radius = 5.f;

public:
  sf::Vector2f pos;
  sf::Vector2f pos_before_mov_to;
  sf::Vector2f velocity{0.f, 0.f};

  // Tuning variables for the physics feel
  float max_speed = 300.f;
  float acceleration = 1000.f;
  float friction = 2.f; // Higher number = stops faster

  bool has_target = false;

  Player() : Sprite() {}

  void update(float dt) {
    float direction_x = 0.f;
    float direction_y = 0.f;

    // 1. Determine Input Direction (Pathfinding vs. Keyboard)
    if (has_target) {
      // Calculate vector to target
      sf::Vector2f to_target = target_pos - pos;
      float distance =
          std::sqrt(to_target.x * to_target.x + to_target.y * to_target.y);

      if (distance > arrival_radius) {
        // Normalize the vector to get a clean direction vector
        direction_x = to_target.x / distance;
        direction_y = to_target.y / distance;
      } else {
        // Arrived at the destination! Stop moving toward it.
        has_target = false;

        // Optional: comment these lines out if you prefer a smooth slide past
        // the exact stopping point
        velocity = {0.f, 0.f};
        pos = target_pos;
      }
    } else {
      // Fallback to classic keyboard input if no target is active
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
          sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        direction_y = -1.f;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
          sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        direction_y = 1.f;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
          sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        direction_x = 1.f;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
          sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        direction_x = -1.f;
    }

    // 2. Apply Acceleration based on direction
    if (direction_x != 0.f) {
      velocity.x += direction_x * acceleration * dt;
    }
    if (direction_y != 0.f) {
      velocity.y += direction_y * acceleration * dt;
    }

    // 3. Apply Friction/Deceleration when input or automatic direction is zero
    if (direction_x == 0.f) {
      velocity.x -= velocity.x * friction * dt;
      if (std::abs(velocity.x) < 0.1f)
        velocity.x = 0.f;
    }
    if (direction_y == 0.f) {
      velocity.y -= velocity.y * friction * dt;
      if (std::abs(velocity.y) < 0.1f)
        velocity.y = 0.f;
    }

    // 4. Cap top speed so your player doesn't accelerate infinitely
    float current_speed =
        std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (current_speed > max_speed) {
      velocity = (velocity / current_speed) * max_speed;
    }

    // 5. Update positions using our calculated velocity
    pos.x += velocity.x * dt;
    pos.y += velocity.y * dt;

    setPosition(pos);
  }

  // Activates smooth movement to a specific coordinate frame
  void move_to(sf::Vector2f to) {
    pos_before_mov_to = getPosition();
    target_pos = to;
    has_target = true;
  }

  // Quick safety utility: Intercepting with manual keyboard input breaks the
  // auto-walk
  void cancel_move_to() { has_target = false; }
};