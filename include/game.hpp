#pragma once
#include <SDL2/SDL.h>

#include <ball.hpp>
#include <brick.hpp>
#include <iostream>
#include <paddle.hpp>
class Game {
 public:
  Game() {}
  void run();

 private:
  void initialize();
  void handle_input();
  void render();
  const int window_width_ = 1000, window_height_ = 800;
  bool running_ = true;
  SDL_Window* window_;
  SDL_Renderer* renderer_;
  Paddle paddle_;
  Ball ball_;
  Brick brick_;
  double delta_time_;
};