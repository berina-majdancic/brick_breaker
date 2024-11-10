#pragma once
#include <SDL2/SDL.h>

#include <array>
#include <ball.hpp>
#include <brick.hpp>
#include <paddle.hpp>
class Game {
 public:
  Game() {}
  void run();

 private:
  void initialize();
  void handle_input();
  void render();
  void initialize_bricks();
  double calculate_delta_time(Uint64 current_time, Uint64 last_time);
  void quit();
  const int window_width_ = 1000, window_height_ = 800;
  bool running_ = true;
  SDL_Window* window_;
  SDL_Renderer* renderer_;
  Paddle paddle_;
  Ball ball_;
  std::array<Brick, 10> brick_;
  double delta_time_;
};