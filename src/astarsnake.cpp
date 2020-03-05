#include "astarsnake.h"
#include <algorithm>
#include <iostream>

const int AStarSnake::_directionDelta[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

void AStarSnake::Update() {
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

void AStarSnake::UpdateHead() {
  if(_path.empty()) return;
  SDL_Point nextPos = _path.back();
  _path.pop_back();
  _head_x = nextPos.x;
  _head_y = nextPos.y;
}

void AStarSnake::updateGoal(const SDL_Point& goal) {
  _goal = goal;
  _openList.clear();
  _path.clear();
  _grid = std::vector<std::vector<Node>>(_gridWidth, std::vector<Node>(_gridHeight, Node()));
  _path = search();
}

std::vector<SDL_Point>& AStarSnake::path() {
  return _path;
}

float AStarSnake::heuristic(const SDL_Point& start, const SDL_Point& end) {
  return abs(end.x - start.x) + abs(end.y - start.y);
}

bool AStarSnake::isValidPos(const int x, const int y) {
  bool validX = (x >= 0 && x < _gridWidth);
  bool validY = (y >= 0 && y < _gridHeight);

  return (validX && validY && !isSnakeCell(x, y));
}

void AStarSnake::expandNeighbors(Node& node) {
  for(int i = 0; i < 4; ++i) {
    int x = node.pos.x + _directionDelta[i][0];
    int y = node.pos.y + _directionDelta[i][1];

    if(isValidPos(x, y)) {
      Node& nextNode = _grid[x][y];
      if(nextNode.state == State::Close || nextNode.visited){
        continue;
      }
      nextNode.pos = {x, y};
      nextNode.gVal = node.gVal + 1;
      nextNode.hVal = heuristic(nextNode.pos, _goal);
      nextNode.parent = &_grid[node.pos.x][node.pos.y];
      nextNode.visited = true;
      if(nextNode.state != State::Start && nextNode.state != State::Goal) {
        nextNode.state = State::Close;
      }
      _openList.emplace_back(nextNode);
    }
  }
}

const std::vector<SDL_Point>& AStarSnake::search() {
  int startX = static_cast<int>(headX());
  int startY = static_cast<int>(headY());
  Node& start = _grid[startX][startY];
  start.pos = {startX, startY};
  start.visited = true;
  start.gVal = 0.0;
  start.hVal = heuristic(start.pos, _goal);
  start.state = State::Start;
  start.parent = nullptr;

  Node& end = _grid[_goal.x][_goal.y];
  end.pos = {_goal.x, _goal.y};
  end.state = State::Goal;

  _openList.emplace_back(start);

  while(!_openList.empty()) {
    std::sort(_openList.begin(), _openList.end(), [](const Node& lhs, const Node& rhs) {
      return (lhs.gVal + lhs.hVal) > (rhs.gVal + rhs.hVal);
    });

    Node& current = _openList.back();
    _openList.pop_back();
      
    if(current.pos.x == _goal.x && current.pos.y == _goal.y) {
      // Search done, return path
      _path.push_back(current.pos);
      Node* pathNode = current.parent;
      if(pathNode == nullptr) {
        // end == start, very corner case
        _path.push_back(start.pos);
        return _path;
      }
      while(pathNode != &start) {
        _path.push_back(pathNode->pos);
        pathNode = pathNode->parent;
      }
      //std::reverse(path.begin(), path.end()); // don't need to reverse as the snake will consume the vector from the back
      return _path;
    }

    expandNeighbors(current);
  }
  return _path;
}