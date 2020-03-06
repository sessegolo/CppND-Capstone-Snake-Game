#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : _snake(grid_width, grid_height),
      _aStarSnake(grid_width, grid_height),
      _engine(_dev()),
      _random_w(0, static_cast<int>(grid_width-1)),
      _random_h(0, static_cast<int>(grid_height-1)) {
  //PlaceFood();
  PlaceFoodAStar();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running && _snake.isAlive()) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, _snake);
    Update();
    renderer.Render(_snake, _food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(_score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
  // show game over?
  renderer.gameOver();
  SDL_Delay(2000);
}

void Game::RunAStar(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running && _aStarSnake.isAlive()) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.

    // sometimes, the food will not appear and the snake will be stuck
    // so pressing ESC will kill the game
    controller.HandleKillSwitch(running);
    Update();
    renderer.Render(_aStarSnake, _food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(_score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
  // show game over?
  renderer.gameOver();
  SDL_Delay(2000);
}

void Game::PlaceFoodAStar() {
  int x, y;
  while (true) {
    x = _random_w(_engine);
    y = _random_h(_engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!_aStarSnake.isSnakeCell(x, y)) {
      _food.x = x;
      _food.y = y;
      _aStarSnake.updateGoal(_food);
      return;
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = _random_w(_engine);
    y = _random_h(_engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!_snake.isSnakeCell(x, y)) {
      _food.x = x;
      _food.y = y;
      return;
    }
  }
}
void Game::Update() {
  if (!_aStarSnake.isAlive()) return;

  _aStarSnake.Update();
  int new_x = static_cast<int>(_aStarSnake.headX());
  int new_y = static_cast<int>(_aStarSnake.headY());

  // Check if there's food over here
  if (_food.x == new_x && _food.y == new_y) {
    _score++;
    // Grow snake and increase speed.
    _aStarSnake.GrowBody();
    _aStarSnake.updateSpeed(0.02);
    //PlaceFood();
    PlaceFoodAStar();
  }
}

int Game::GetScore() const { return _score; }
int Game::GetSize() const { return _aStarSnake.size(); }