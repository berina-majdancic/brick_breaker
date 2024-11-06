#pragma once
#include <SDL2/SDL.h>
enum class Direction { left = -1, right = 1 };
class Paddle {
 public:
  Paddle(int window_width, int window_height) {
    window_width_ = window_width;
    window_height_ = window_height;
    width_ = 100;
    height_ = 20;
    x_ = (window_width_ - width_) / 2;
    y_ = (window_height_ - height_) - 20;
    speed_ = 15;
    rect_ = {x_, y_, width_, height_};
  }
  void render(SDL_Renderer* renderer);
  void move(Direction direction);

 private:
  int window_width_, window_height_;
  int width_, height_;
  int x_, y_;
  int speed_;
  SDL_Rect rect_;
};