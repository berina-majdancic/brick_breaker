#include <ball.hpp>

void Ball::render() {
  const int diameter = (radius_ * 2);

  int x = (radius_ - 1);
  int y = 0;
  int tx = 1;
  int ty = 1;
  int error = (tx - diameter);

  while (x >= y) {
    // render left and right side
    for (int i = centre_y_ - y; i <= centre_y_ + y; i++) {
      SDL_RenderDrawPoint(renderer_, centre_x_ + x, i);
      SDL_RenderDrawPoint(renderer_, centre_x_ - x, i);
    }

    if (x != y) {
      for (int i = centre_y_ - x; i <= centre_y_ + x; i++) {
        SDL_RenderDrawPoint(renderer_, centre_x_ + y, i);
        SDL_RenderDrawPoint(renderer_, centre_x_ - y, i);
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
  handle_brick_collision();
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

bool Ball::check_collision(const SDL_Rect& rect) {
  int closest_x, closest_y;

  // Find the closest x point
  if (centre_x_ < rect.x) {
    closest_x = rect.x;
  } else if (centre_x_ > rect.x + rect.w) {
    closest_x = rect.x + rect.w;
  } else {
    closest_x = centre_x_;
  }

  // Find the closest y point
  if (centre_y_ < rect.y) {
    closest_y = rect.y;
  } else if (centre_y_ > rect.y + rect.h) {
    closest_y = rect.y + rect.h;
  } else {
    closest_y = centre_y_;
  }

  int distance_x = centre_x_ - closest_x;
  int distance_y = centre_y_ - closest_y;
  float distance = sqrt((distance_x * distance_x) + (distance_y * distance_y));
  // If the closest point is inside the circle, return true
  return distance < radius_;
}

void Ball::handle_paddle_collision() {
  if (check_collision(paddle_->get_rect())) {
    change_angle(paddle_->get_rect());
  }
}

void Ball::handle_brick_collision() {
  if (brick_->is_alive() && check_collision(brick_->get_rect())) {
    change_angle(brick_->get_rect());
    brick_->damage();
  }
}
void Ball::change_angle(const SDL_Rect& rect) {
  int rect_center_x = rect.x + (rect.w / 2);
  int distance_from_center = centre_x_ - rect_center_x;

  // Calculate a horizontal factor based on how far the ball hit from the
  // center
  float max_angle = 75.0f;
  float hit_ratio = static_cast<float>(distance_from_center) / (rect.w / 2);
  float angle_radians = (max_angle * hit_ratio) * (M_PI / 180.0f);

  // Update based on the new angle
  float speed = sqrt((speed_x_ * speed_x_) + (speed_y_ * speed_y_));
  speed_x_ = speed * sin(angle_radians);
  speed_y_ = -speed * cos(angle_radians);
}
