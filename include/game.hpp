#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

#include <array>
#include <ball.hpp>
#include <brick.hpp>
#include <paddle.hpp>
#include <string>
enum class MenuItem{ start = 0, exit = 1};
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
  void handle_menu_input();
  void render_menu();
  void display_text(const std::string& text, int x, int y, int size, SDL_Color color, bool centered);
  const int window_width_ = 1920, window_height_ = 1080;
  bool running_ = true;
  SDL_Window* window_;
  SDL_Renderer* renderer_;
  Paddle paddle_;
  Ball ball_;
  std::array<Brick, NUM_OF_BRICKS> brick_;
  double delta_time_;
  TTF_Font* font_;
  SDL_Texture* texture_;
  SDL_Texture* background_texture_;
  bool in_menu_ = true;
  bool game_in_progress_ = false;
  int selected_menu_item_ = 0;
};