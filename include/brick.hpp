#pragma once
#include <SDL2/SDL.h>
class Brick {
 public:
  Brick() {};
  Brick(SDL_Renderer* renderer, int x, int y) {
    width_ = 70;
    height_ = 20;
    x_ = x;
    y_ = y;
    rect_ = {x_, y_, width_, height_};
    renderer_ = renderer;
  }
  void render();
  int get_x() { return x_; }
  int get_y() { return y_; }
  int get_width() { return width_; }
  int get_height() { return height_; }
  SDL_Rect get_rect() { return rect_; }
  bool is_alive() { return is_alive_; }
  void damage() {
    health_--;
    if (health_ <= 0) is_alive_ = false;
  }

 private:
  int x_, y_;
  bool is_alive_ = true;
  int health_ = 1;
  int width_, height_;
  SDL_Renderer* renderer_;
  SDL_Rect rect_;
};