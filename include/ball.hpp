#pragma once
#include <SDL2/SDL.h>

#include <paddle.hpp>
class Ball {
 public:
  Ball() {}
  Ball(int window_width, int window_height, Paddle* paddle,
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
  void handle_window_collision();
  void handle_paddle_collision();
  double distance_squared(int x1, int y1, int x2, int y2);
  bool check_collision(int ball_x, int ball_y, int ball_radius,
                       const SDL_Rect& rect);
  int window_height_, window_width_;
  int centre_x_, centre_y_;
  int radius_;
  double speed_x_ = 1, speed_y_ = -1;
  Paddle* paddle_;
  SDL_Renderer* renderer_;
};