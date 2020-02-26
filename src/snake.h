#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height)
      : _grid_width(grid_width),
        _grid_height(grid_height),
        _head_x(grid_width / 2),
        _head_y(grid_height / 2),
        _direction{Direction::kUp},
        _speed{0.1f},
        _size{1},
        _alive{true},
        _growing{false},
        _body{} {}

  void Update();

  void GrowBody();
  bool isSnakeCell(int x, int y);

  Direction direction() const;
  void updateDirection(const Direction newDirection);

  float speed() const;
  void updateSpeed(const float speed);

  int size() const;
  void updateSize(const int size);
  
  bool isAlive() const;
  void updateAlive(const bool alive);
  
  // Only getters as the head position shall not be modified externally
  float headX() const;
  float headY() const;
  
  const std::vector<SDL_Point>& body() const;

 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  int _grid_width;
  int _grid_height;
  float _head_x;
  float _head_y;
  Direction _direction;
  float _speed;
  int _size;
  bool _alive;
  bool _growing;
  std::vector<SDL_Point> _body;
};

#endif