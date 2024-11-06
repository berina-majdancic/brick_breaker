#include <SDL2/SDL.h>
enum class Direction { left = -1, right = 1 };
class Paddle {
 public:
  Paddle(int screen_width, int screen_height) {
    screen_width_ = screen_width;
    screen_height_ = screen_height;
    width_ = 100;
    height_ = 20;
    x_ = (screen_width_ - width_) / 2;
    y_ = (screen_height_ - height_) - 20;
    speed_ = 15;
    rect_ = {x_, y_, width_, height_};
  }
  void render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect_);
  }
  void move(Direction direction) {
    x_ += static_cast<int>(direction) * speed_;
    if (x_ < 0)
      x_ = 0;
    else if (x_ > screen_width_ - width_)
      x_ = screen_width_ - width_;
    rect_.x = x_;
  }

 private:
  int screen_width_, screen_height_;
  int width_, height_;
  int x_, y_;
  int speed_;
  SDL_Rect rect_;
};