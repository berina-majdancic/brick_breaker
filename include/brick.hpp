#pragma once
#include <SDL.h>
#define NUM_OF_BRICKS 70

class Brick {
 public:
  Brick() {};
  Brick(SDL_Renderer* renderer, int x, int y, int health)
      : width_(130),
        height_(40),
        x_(x),
        y_(y),
        rect_{x, y, width_, height_},
        renderer_(renderer),
        health_(health),
        initial_health_(health) {}
  bool render();
  int get_x() { return x_; }
  int get_y() { return y_; }
  int get_width() { return width_; }
  int get_height() { return height_; }
  void set_x(int x) {
    rect_.x = x;
    x_ = x;
  }
  void set_y(int y) {
    rect_.y = y;
    y_ = y;
  }
  void set_health(int health) { health_ = health; }
  SDL_Rect get_rect() { return rect_; }
  bool is_alive() { return health_ > 0; }
  void damage();
  void reset();
  static void load_texture(SDL_Renderer* renderer);

 private:
  int x_, y_;
  int health_;
  int width_, height_;
  SDL_Renderer* renderer_;
  SDL_Rect rect_;
  static SDL_Texture* texture_;
  int initial_health_;
};
