#include <ball.hpp>
#include <cmath>
#include <iostream>
#include <SDL_image.h>

void Ball::render() {
    rect_.x = centre_x_ - radius_;  
    rect_.y = centre_y_ - radius_;  
    rect_.w = radius_*2;
    rect_.h = radius_ * 2;
    SDL_RenderCopy(renderer_, texture_, nullptr, &rect_);
    /*
  const int diameter = (radius_ * 2);
  int x = (radius_ - 1);
  int y = 0;
  int tx = 1;
  int ty = 1;
  int error = (tx - diameter);

  while (x >= y) {
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
  }*/
}

void Ball::load_texture() {
    SDL_Surface* tempSurface = IMG_Load("assets/images/ball.png");
    if (!tempSurface) {
    }
    texture_ = SDL_CreateTextureFromSurface(renderer_, tempSurface);

    SDL_FreeSurface(tempSurface);
}
void Ball::move(double& delta_time) {
  centre_x_ += static_cast<int>(speed_x_ * delta_time);
  centre_y_ += static_cast<int>(speed_y_ * delta_time);

  handle_window_collision();
  handle_paddle_collision();
  for (int i = 0; i < NUM_OF_BRICKS; i++) {
    handle_brick_collision(&(*brick_array_)[i]);
  }
}

void Ball::handle_window_collision() {
  // X
  if (centre_x_ - radius_ < 0) {
    centre_x_ = radius_;
    speed_x_ = -speed_x_;
  } else if (centre_x_ + radius_ > window_width_) {
    centre_x_ = window_width_ - radius_;
    speed_x_ = -speed_x_;
  }

  // Y
  if (centre_y_ - radius_ < 0) {
    centre_y_ = radius_;
    speed_y_ = -speed_y_;
  } else if (centre_y_ + radius_ > window_height_) {
    centre_y_ = window_height_ - radius_;
    speed_y_ = -speed_y_;
  }
}
bool Ball::detect_collision(const SDL_Rect& rect) {
  int ball_left = centre_x_ - radius_;
  int ball_right = centre_x_ + radius_;
  int ball_top = centre_y_ - radius_;
  int ball_bottom = centre_y_ + radius_;

  int rect_left = rect.x;
  int rect_right = rect.x + rect.w;
  int rect_top = rect.y;
  int rect_bottom = rect.y + rect.h;

  bool is_collision = (ball_right > rect_left && ball_left < rect_right &&
                       ball_bottom > rect_top && ball_top < rect_bottom);

  if (is_collision) {
    std::cout << "Collision detected!" << std::endl;

    int overlap_left = ball_right - rect_left;
    int overlap_right = rect_right - ball_left;
    int overlap_top = ball_bottom - rect_top;
    int overlap_bottom = rect_bottom - ball_top;

    int min_overlap = std::min(std::min(overlap_left, overlap_right),
                               std::min(overlap_top, overlap_bottom));

    if (min_overlap == overlap_left) {
      side_hit_ = Side::LEFT;
      std::cout << "Left" << std::endl;

    } else if (min_overlap == overlap_right) {
      side_hit_ = Side::RIGHT;
      std::cout << "Right" << std::endl;

    } else if (min_overlap == overlap_top) {
      side_hit_ = Side::TOP;
      std::cout << "Top" << std::endl;

    } else if (min_overlap == overlap_bottom) {
      side_hit_ = Side::BOTTOM;
      std::cout << "Bottom" << std::endl;
    }
  }
  return is_collision;
}
void Ball::handle_paddle_collision() {
  if (detect_collision(paddle_->get_rect())) {
    change_angle(paddle_->get_rect(), side_hit_);
  }
}

void Ball::handle_brick_collision(Brick* brick) {
  if (brick->is_alive() && detect_collision(brick->get_rect())) {
    change_angle(brick->get_rect(), side_hit_);
    brick->damage();
    if (!brick->is_alive()) score_ += 10;
  }
}
void Ball::change_angle(const SDL_Rect& rect, Side side) {
  int rect_center_x = rect.x + (rect.w / 2);
  int distance_from_center = centre_x_ - rect_center_x;

  double max_angle = 75.0f;
  double hit_ratio = static_cast<double>(distance_from_center) / (rect.w / 2);
  double angle_radians = (max_angle * hit_ratio) * (M_PI / 180.0f);

  double speed = sqrt((speed_x_ * speed_x_) + (speed_y_ * speed_y_));

  switch (side) {
    case Side::TOP:
      speed_y_ = -abs(speed * sin(angle_radians));
      speed_x_ = speed_x_ > 0 ? speed * cos(angle_radians)
                              : -speed * cos(angle_radians);
      break;

    case Side::BOTTOM:
      speed_y_ = abs(speed * sin(angle_radians));
      speed_x_ = speed_x_ > 0 ? speed * cos(angle_radians)
                              : -speed * cos(angle_radians);
      break;

    case Side::LEFT:
      speed_x_ = -abs(speed_x_);
      break;

    case Side::RIGHT:
      speed_x_ = abs(speed_x_);
      break;
  }
}
