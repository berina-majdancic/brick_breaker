#include <paddle.hpp>

void Paddle::render(SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &rect_);
}
void Paddle::move(Direction direction) {
  x_ += static_cast<int>(direction) * speed_;
  if (x_ < 0)
    x_ = 0;
  else if (x_ > window_width_ - width_)
    x_ = window_width_ - width_;
  rect_.x = x_;
}