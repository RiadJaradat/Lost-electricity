#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

#include "Sprite.hpp"

class Player : public Sprite {
public:
  sf::Vector2f pos;
  sf::Vector2f velocity{0.f, 0.f};

  // Tuning variables for the physics feel
  float max_speed = 300.f;
  float acceleration = 1000.f;
  float friction = 2.f; // Higher number = stops faster

  Player() : Sprite() {}

  void update(float dt) {
    // 1. Get movement direction from input
    float direction_x = 0.f;
    float direction_y = 0.f;

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

    // 2. Apply Acceleration based on input
    if (direction_x != 0.f) {
      velocity.x += direction_x * acceleration * dt;
    }
    if (direction_y != 0.f) {
      velocity.y += direction_y * acceleration * dt;
    }

    // 3. Apply Friction/Deceleration when input is zero
    if (direction_x == 0.f) {
      // Slide down toward zero
      velocity.x -= velocity.x * friction * dt;
      // Snap to zero if it gets tiny to prevent endless micro-drifting
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
};