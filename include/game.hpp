#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

#include <array>
#include <ball.hpp>
#include <brick.hpp>
#include <paddle.hpp>
#include <string>
class Game {
 public:
  void run();

 private:
  void initialize();
  void handle_input();
  void render();
  void initialize_bricks();
  void quit();
  void load_bckground();
  void render_score();
  void handle_menu_input();
  void render_menu();
  void game_reset();
  void game_over();
  void game_won();
  bool init_audio();
  bool load_wav_file(const char* file);
  inline void play_game_over_audio();
  inline void play_ball_hit_audio();
  inline void play_game_start_audio();
  void display_text(const std::string& text, int x, int y, int size,
                    SDL_Color color, bool centered);
  double calculate_delta_time(Uint64 current_time, Uint64 last_time);
  const int window_width_ = 1920, window_height_ = 1080;
  bool running_ = true;
  SDL_Window* window_;
  SDL_Renderer* renderer_;
  Paddle paddle_;
  Ball ball_;
  std::array<Brick, NUM_OF_BRICKS> brick_;
  double delta_time_;
  TTF_Font* font_;
  SDL_Texture* background_texture_;
  bool in_menu_ = true;
  bool game_in_progress_ = false;
  int selected_menu_item_ = 0;
  int bricks_alive_ = NUM_OF_BRICKS;
  Uint8* audio_buffer_;
  Uint32 audio_length_;
  SDL_AudioSpec audio_spec_;
  SDL_AudioDeviceID audio_device_;
};