#pragma once
#include <SDL2/SDL.h>
class Brick {
 public:
  Brick() {};
  Brick(SDL_Renderer* renderer) {
    width_ = 30;
    height_ = 20;
    x_ = 500 - width_;
    y_ = 20;
    rect_ = {x_, y_, width_, height_};
    renderer_ = renderer;
  }
  void render();
  int get_x() { return x_; }
  int get_y() { return y_; }
  int get_width() { return width_; }
  int get_height() { return height_; }
  SDL_Rect get_rect() { return rect_; }

 private:
  int x_, y_;
  int width_, height_;
  SDL_Renderer* renderer_;
  SDL_Rect rect_;
};