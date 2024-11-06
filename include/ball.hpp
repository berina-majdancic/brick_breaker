#pragma once
#include <SDL2/SDL.h>

#include <paddle.hpp>
class Ball {
 public:
  Ball(int window_width, int window_height) {
    window_width_ = window_width;
    window_height_ = window_height;
    centre_x_ = window_width / 2;
    centre_y_ = window_height - 150;
    radius_ = 8;
  }
  void render(SDL_Renderer* renderer);
  void move(Paddle& paddle);

 private:
  int centre_x_, centre_y_;
  int window_height_, window_width_;
  int radius_;
  int speed_x_ = 1, speed_y_ = -1;
};