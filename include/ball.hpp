#pragma once
#include <SDL.h>

#include <array>
#include <brick.hpp>
#include <paddle.hpp>
enum class Side_hit { TOP, BOTTOM, LEFT, RIGHT, NONE };
class Ball {
 public:
  Ball() {}
  Ball(int window_width, int window_height, Paddle* paddle,
       std::array<Brick, NUM_OF_BRICKS>* brick_array, SDL_Renderer* renderer) {
    window_width_ = window_width;
    window_height_ = window_height;
    centre_x_ = window_width / 2;
    centre_y_ = window_height - 150;
    paddle_ = paddle;
    brick_array_ = brick_array;
    renderer_ = renderer;
  }
  void render();
  void move(double& delta_time);

 private:
  void handle_window_collision();
  void handle_paddle_collision();
  void handle_brick_collision(Brick* brick);
  void change_angle(const SDL_Rect& rect, Side_hit side);
  bool check_collision(const SDL_Rect& rect);

  int window_height_, window_width_;
  int centre_x_, centre_y_;
  int radius_ = 8;
  Side_hit side_hit_;
  double speed_x_ = 0.9, speed_y_ = -0.9;
  Paddle* paddle_;
  std::array<Brick, NUM_OF_BRICKS>* brick_array_;
  SDL_Renderer* renderer_;
};