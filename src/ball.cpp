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

void Ball::move(double& delta_time) {
  centre_x_ += static_cast<int>(speed_x_ * delta_time);
  centre_y_ += static_cast<int>(speed_y_ * delta_time);

  handle_window_collision();
  handle_paddle_collision();
}

void Ball::handle_window_collision() {
  // Handle X-axis
  if (centre_x_ - radius_ < 0) {
    centre_x_ = radius_;
    speed_x_ = -speed_x_;
  } else if (centre_x_ + radius_ > window_width_) {
    centre_x_ = window_width_ - radius_;
    speed_x_ = -speed_x_;
  }

  // Handle Y-axis
  if (centre_y_ - radius_ < 0) {
    centre_y_ = radius_;
    speed_y_ = -speed_y_;
  } else if (centre_y_ + radius_ > window_height_) {
    centre_y_ = window_height_ - radius_;
    speed_y_ = -speed_y_;
  }
}
double Ball::distance_squared(int x1, int y1, int x2, int y2) {
  int delta_x = x2 - x1;
  int delta_y = y2 - y1;
  return delta_x * delta_x + delta_y * delta_y;
}

bool Ball::check_collision(int ball_x, int ball_y, int ball_radius,
                           const SDL_Rect& rect) {
  int closest_x, closest_y;

  // Find the closest x point
  if (ball_x < rect.x) {
    closest_x = rect.x;
  } else if (ball_x > rect.x + rect.w) {
    closest_x = rect.x + rect.w;
  } else {
    closest_x = ball_x;
  }

  // Find the closest y point
  if (ball_y < rect.y) {
    closest_y = rect.y;
  } else if (ball_y > rect.y + rect.h) {
    closest_y = rect.y + rect.h;
  } else {
    closest_y = ball_y;
  }

  // If the closest point is inside the circle, return true
  return distance_squared(ball_x, ball_y, closest_x, closest_y) <
         ball_radius * ball_radius;
}

void Ball::handle_paddle_collision() {
  if (check_collision(centre_x_, centre_y_, radius_, paddle_->get_rect())) {
    // Handle the collision response (bounce the ball)
    speed_y_ = -speed_y_;  // Reverse the vertical speed to simulate a bounce

    // Optionally, adjust the ball's position so it doesn't go through the
    // paddle
    centre_y_ = paddle_->get_y() - radius_;
  }
}