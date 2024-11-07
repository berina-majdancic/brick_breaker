#include <ball.hpp>

void Ball::render() {
  const int diameter = (radius_ * 2);

  int x = (radius_ - 1);
  int y = 0;
  int tx = 1;
  int ty = 1;
  int error = (tx - diameter);

  while (x >= y) {
    for (int i = centre_y_ - y; i <= centre_y_ + y; i++) {
      SDL_RenderDrawPoint(renderer_, centre_x_ + x,
                          i);  // Right side of the circle
      SDL_RenderDrawPoint(renderer_, centre_x_ - x,
                          i);  // Left side of the circle
    }

    if (x != y) {
      for (int i = centre_y_ - x; i <= centre_y_ + x; i++) {
        SDL_RenderDrawPoint(renderer_, centre_x_ + y,
                            i);  // Right side of the circle
        SDL_RenderDrawPoint(renderer_, centre_x_ - y,
                            i);  // Left side of the circle
      }
    }

    if (error <= 0) {
      ++y;
      error += ty;
      ty += 2;
    }

    if (error > 0) {
      --x;
      tx += 2;
      error += (tx - diameter);
    }
  }
}

void Ball::move(double &delta_time) {
  centre_x_ += static_cast<int>(speed_x_ * delta_time);
  if (centre_x_ - radius_ <= 0 || centre_x_ + radius_ >= window_width_) {
    centre_x_ -= static_cast<int>(speed_x_ * delta_time);
    speed_x_ = -(speed_x_);
  }
  centre_y_ += static_cast<int>(speed_y_ * delta_time);
  if (centre_y_ - radius_ <= 0 || centre_y_ + radius_ >= window_height_) {
    centre_y_ -= static_cast<int>(speed_y_ * delta_time);
    speed_y_ = -(speed_y_);
  }
  if (centre_x_ + radius_ >= paddle_.get_x() &&
      centre_y_ + radius_ >= paddle_.get_y()) {
    speed_x_ = -(speed_x_);
    speed_y_ = -(speed_y_);
  }
}