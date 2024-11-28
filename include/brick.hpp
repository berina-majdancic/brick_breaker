#pragma once
#include <SDL.h>
#define NUM_OF_BRICKS 125

class Brick {
 public:
  Brick(){};
  Brick(SDL_Renderer* renderer, int x, int y, int health) {
    width_ = 100;
    height_ = 40;
    x_ = x;
    y_ = y;
    rect_ = {x_, y_, width_, height_};
    renderer_ = renderer;
    health_ = health;
    initial_health_ = health;
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
  void reset();
  static void load_texture(SDL_Renderer* renderer);

 private:
  int x_, y_;
  bool is_alive_ = true;
  int health_;
  int width_, height_;
  SDL_Renderer* renderer_;
  SDL_Rect rect_;
  static SDL_Texture* texture_;
  int initial_health_;
};
