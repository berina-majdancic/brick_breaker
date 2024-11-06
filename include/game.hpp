#pragma once
#include <SDL2/SDL.h>

#include <iostream>

class Game {
 public:
  Game() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
                << std::endl;
    }
  }
  void start();

 private:
  void create_window();
  void renderer_init();
  const int window_width_ = 1000, window_height_ = 800;
  bool running_ = false;
  SDL_Window* window_;
  SDL_Renderer* renderer_;
};