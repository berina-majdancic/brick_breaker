#include <SDL_image.h>

#include <paddle.hpp>
void Paddle::render() { SDL_RenderCopy(renderer_, texture_, nullptr, &rect_); }
void Paddle::move(Direction direction, double& delta_time) {
  x_ += static_cast<int>(direction) * static_cast<int>(speed_ * delta_time);
  if (x_ < 0)
    x_ = 0;
  else if (x_ > window_width_ - width_)
    x_ = window_width_ - width_;
  rect_.x = x_;
}
void Paddle::load_texture() {
  SDL_Surface* tempSurface = IMG_Load("assets/images/paddle.png");
  if (!tempSurface) {
  }
  texture_ = SDL_CreateTextureFromSurface(renderer_, tempSurface);

  SDL_FreeSurface(tempSurface);
}

void Paddle::reset() {
  x_ = (window_width_ - width_) / 2;
  y_ = (window_height_ - height_) - 40;
  rect_ = {x_, y_, width_, height_};
}