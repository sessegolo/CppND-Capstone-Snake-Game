#include "snake.h"
#include <cmath>
#include <iostream>
#include <algorithm>

void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(_head_x),
      static_cast<int>(_head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(_head_x),
      static_cast<int>(_head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void Snake::UpdateHead() {
  switch (_direction) {
    case Direction::kUp:
      _head_y -= speed();
      break;

    case Direction::kDown:
      _head_y += speed();
      break;

    case Direction::kLeft:
      _head_x -= speed();
      break;

    case Direction::kRight:
      _head_x += speed();
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  _head_x = fmod(_head_x + _grid_width, _grid_width);
  _head_y = fmod(_head_y + _grid_height, _grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  _body.push_back(prev_head_cell);

  if (!_growing) {
    // Remove the tail from the vector.
    _body.erase(_body.begin());
  } else {
    _growing = false;
    updateSize(1);
  }

  // Check if the snake has died.
  for (auto const &item : _body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      updateAlive(false);
    }
  }
}

void Snake::GrowBody() { _growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::isSnakeCell(int x, int y) {
  if (x == static_cast<int>(_head_x) && y == static_cast<int>(_head_y)) {
    return true;
  }
  auto it = std::find_if(_body.begin(), _body.end(), [x, y](const SDL_Point& p) {
    return p.x == x && p.y == y;
  });
  return it != _body.end();
}

Snake::Direction Snake::direction() const {
  return _direction;
}

void Snake::updateDirection(const Snake::Direction newDirection) {
  _direction = newDirection;
}

float Snake::speed() const {
  return _speed;
}

void Snake::updateSpeed(const float speed) {
  _speed += speed;
}

int Snake::size() const {
  return _size;
}

void Snake::updateSize(const int size) {
  _size += size;
}

bool Snake::isAlive() const {
  return _alive;
}

void Snake::updateAlive(const bool alive) {
  _alive = alive;
}

float Snake::headX() const {
  return _head_x;
}

float Snake::headY() const {
  return _head_y;
}

const std::vector<SDL_Point>& Snake::body() const {
  return _body;
}