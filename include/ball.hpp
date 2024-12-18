#pragma once
#include <SDL.h>

#include <array>
#include <brick.hpp>
#include <paddle.hpp>
enum class Side { TOP, BOTTOM, LEFT, RIGHT, NONE };
class Ball {
 public:
  Ball() {}
  Ball(int window_width, int window_height, Paddle* paddle,
       std::array<Brick, NUM_OF_BRICKS>* brick_array, SDL_Renderer* renderer)
      : window_width_(window_width),
        window_height_(window_height),
        centre_x_(window_width / 2),
        centre_y_(window_height - 150),
        paddle_(paddle),
        brick_array_(brick_array),
        renderer_(renderer) {
    load_texture();
  }
  void render();
  void move(double& delta_time);
  int get_score() { return score_; }
  bool hit_bottom() { return hit_bottom_; }
  void reset();

 private:
  void handle_window_collision();
  void handle_paddle_collision();
  void handle_brick_collision(Brick* brick);
  void change_angle(const SDL_Rect& rect, Side side);
  bool detect_collision(const SDL_Rect& rect);
  void load_texture();
  int window_height_, window_width_;
  int centre_x_, centre_y_;
  int radius_ = 14;
  Side side_hit_;
  float speed_x_ = 0, speed_y_ = 0.9;
  Paddle* paddle_;
  std::array<Brick, NUM_OF_BRICKS>* brick_array_;
  unsigned int score_ = 0;
  SDL_Renderer* renderer_;
  SDL_Texture* texture_;
  SDL_Rect rect_;
  bool hit_bottom_ = false;
};