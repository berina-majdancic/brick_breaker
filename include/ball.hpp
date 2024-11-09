#pragma once
#include <SDL2/SDL.h>

#include <brick.hpp>
#include <paddle.hpp>
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
  double distance_squared(int x1, int y1, int x2, int y2);
  bool check_collision(const SDL_Rect& rect);
  int window_height_, window_width_;
  int centre_x_, centre_y_;
  int radius_;
  double speed_x_ = 1, speed_y_ = -1;
  Paddle* paddle_;
  Brick* brick_;
  SDL_Renderer* renderer_;
};