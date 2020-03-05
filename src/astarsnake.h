#ifndef ASTARSNAKE_H
#define ASTARSNAKE_H

#include "snake.h"
#include <vector>
#include <limits>

enum class State {
  Empty,
  Open,
  Close,
  Start,
  Goal
};

struct Node {
  Node() : 
    pos{0,0}, 
    visited(false), 
    gVal(.0), 
    hVal(std::numeric_limits<float>::max()), 
    state(State::Empty),
    parent(nullptr) {}
  SDL_Point pos;
  bool visited;
  float gVal;
  float hVal;
  State state;
  Node *parent;
};

class AStarSnake : public Snake {
public:
  AStarSnake(int grid_width, int grid_height) : Snake(grid_width, grid_height),
    _gridWidth(grid_width),
    _gridHeight(grid_height),
    _openList{}, 
    _grid(grid_width, std::vector<Node>(grid_height, Node())),
    _goal(),
    _path{} {}

  void Update();

  void updateGoal(const SDL_Point& pos);
  const std::vector<SDL_Point>& search();
  std::vector<SDL_Point>& path();
private:
  void UpdateHead();
  float heuristic(const SDL_Point& start, const SDL_Point& end);
  void expandNeighbors(Node& node);
  bool isValidPos(const int x, const int y);
  int _gridWidth;
  int _gridHeight;
  std::vector<Node> _openList;
  std::vector<std::vector<Node>> _grid;
  SDL_Point _goal;
  std::vector<SDL_Point> _path;
  const static int _directionDelta[4][2];
};

#endif