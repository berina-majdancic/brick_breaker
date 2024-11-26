#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

#include <array>
#include <ball.hpp>
#include <brick.hpp>
#include <paddle.hpp>

class Game {
 public:
  void run();

 private:
  void initialize();
  void handle_input();
  void render();
  void initialize_bricks();
  double calculate_delta_time(Uint64 current_time, Uint64 last_time);
  void quit();
  void load_bckground();
  void render_score();
  const int window_width_ = 1920, window_height_ = 1080;
  bool running_ = true;
  int score;
  SDL_Window* window_;
  SDL_Renderer* renderer_;
  Paddle paddle_;
  Ball ball_;
  std::array<Brick, NUM_OF_BRICKS> brick_;
  double delta_time_;
  TTF_Font* font_;
  SDL_Texture* texture_;
  SDL_Texture* background_texture_;
};