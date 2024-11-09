#pragma once
#include <SDL2/SDL.h>

#include <brick.hpp>
#include <paddle.hpp>
#include <vector>
class Ball {
 public:
  Ball() {}
  Ball(int window_width, int window_height, Paddle* paddle, Brick* brick,
       SDL_Renderer* renderer) {
    window_width_ = window_width;
    window_height_ = window_height;
    centre_x_ = window_width / 2;
    centre_y_ = window_height - 150;
    radius_ = 8;
    paddle_ = paddle;
    brick_ = brick;
    renderer_ = renderer;
  }
  void render();
  void move(double& delta_time);

 private:
  void handle_window_collision();
  void handle_paddle_collision();
  void handle_brick_collision();
  void change_angle(const SDL_Rect& rect);
  bool check_collision(const SDL_Rect& rect);

  int window_height_, window_width_;
  int centre_x_, centre_y_;
  int radius_;
  double speed_x_ = 0.9, speed_y_ = -0.9;
  Paddle* paddle_;
  Brick* brick_;
  SDL_Renderer* renderer_;
};