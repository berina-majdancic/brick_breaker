#include <paddle.hpp>

void Paddle::render() {
  SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer_, &rect_);
}
void Paddle::move(Direction direction, double &delta_time) {
  x_ += static_cast<int>(direction) * static_cast<int>(speed_ * delta_time);
  if (x_ < 0)
    x_ = 0;
  else if (x_ > window_width_ - width_)
    x_ = window_width_ - width_;
  rect_.x = x_;
}