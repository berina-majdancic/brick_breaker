#pragma once
#include <SDL.h>
enum class Direction { left = -1, right = 1 };
class Paddle {
 public:
  Paddle() {}
  Paddle(int window_width, int window_height, SDL_Renderer* renderer) {
    window_width_ = window_width;
    window_height_ = window_height;
    width_ = 200;
    height_ = 30;
    x_ = (window_width_ - width_) / 2;
    y_ = (window_height_ - height_) - 40;
    speed_ = 2;
    rect_ = {x_, y_, width_, height_};
    renderer_ = renderer;
    load_texture();
  }
  void render();
  void move(Direction direction, double& delta_time);
  int get_x() { return x_; }
  int get_y() { return y_; }
  int get_width() { return width_; }
  int get_height() { return height_; }
  SDL_Rect get_rect() { return rect_; }
 private:
  void Paddle::load_texture();
  int window_width_, window_height_;
  int width_, height_;
  int x_, y_;
  double speed_;
  SDL_Rect rect_;
  SDL_Renderer* renderer_;
  SDL_Texture* texture_;
};