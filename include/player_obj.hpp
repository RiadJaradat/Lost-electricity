#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

class Player : public sf::Sprite {
private:
  sf::Vector2f target_pos;
  float arrival_radius = 5.f;

public:
  sf::Vector2f pos;
  sf::Vector2f pos_before_mov_to;
  sf::Vector2f velocity{0.f, 0.f};

  float max_speed = 300.f;
  float acceleration = 1000.f;
  float friction = 2.f; // Higher number = stops faster

  int wheat_count = 0;
  int apple_count = 0;

  bool has_target = false;

  Player() : Sprite() {}

  sf::Vector2f updateDirection() {
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
    return {direction_x, direction_y};
  }

  void update(float dt) {
    sf::Vector2f direction = {0.f, 0.f};

    if (!has_target) {
      direction = updateDirection();

    } else {
      sf::Vector2f to_target = target_pos - pos;
      float distance =
          std::sqrt(to_target.x * to_target.x + to_target.y * to_target.y);

      if (distance > arrival_radius) {
        direction.x = to_target.x / distance;
        direction.y = to_target.y / distance;
      } else {
        has_target = false;
        velocity = {0.f, 0.f};
        pos = target_pos;
      }
    }

    if (direction.x != 0.f) {
      velocity.x += direction.x * acceleration * dt;
    }
    if (direction.y != 0.f) {
      velocity.y += direction.y * acceleration * dt;
    }

    if (direction.x == 0.f) {
      velocity.x -= velocity.x * friction * dt;
      if (std::abs(velocity.x) < 0.1f)
        velocity.x = 0.f;
    }
    if (direction.y == 0.f) {
      velocity.y -= velocity.y * friction * dt;
      if (std::abs(velocity.y) < 0.1f)
        velocity.y = 0.f;
    }

    float current_speed =
        std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (current_speed > max_speed) {
      velocity = (velocity / current_speed) * max_speed;
    }

    pos.x += velocity.x * dt;
    pos.y += velocity.y * dt;

    setPosition(pos);
  }

  void move_to(sf::Vector2f to) {
    pos_before_mov_to = getPosition();
    target_pos = to;
    has_target = true;
  }

  void cancel_move_to() { has_target = false; }
};