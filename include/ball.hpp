#pragma once
#include <SDL2/SDL.h>

#include <paddle.hpp>
class Ball {
 public:
  Ball() {}
  Ball(int window_width, int window_height, Paddle& paddle,
       SDL_Renderer* renderer) {
    window_width_ = window_width;
    window_height_ = window_height;
    centre_x_ = window_width / 2;
    centre_y_ = window_height - 150;
    radius_ = 8;
    paddle_ = paddle;
    renderer_ = renderer;
  }
  void render();
  void move(double& delta_time);

 private:
  int centre_x_, centre_y_;
  int window_height_, window_width_;
  int radius_;
  double speed_x_ = 1, speed_y_ = -1;
  Paddle paddle_;
  SDL_Renderer* renderer_;
};