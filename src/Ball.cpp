#include "Ball.hpp"
#include "Game.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>

void Ball::render() {
    SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 255);
    for (int w = 0; w < radius_ * 2; w++) {
        for (int h = 0; h < radius_ * 2; h++) {
            int dx = radius_ - w;
            int dy = radius_ - h;
            if ((dx * dx + dy * dy) <= (radius_ * radius_)) {
                SDL_RenderDrawPoint(renderer_, static_cast<int>(x_) + dx, static_cast<int>(y_) + dy);
            }
        }
    }
}

void Ball::move(double delta_time) {
    x_ += speed_x_ * delta_time;
    y_ += speed_y_ * delta_time;

    handle_collisions();
}

void Ball::handle_collisions() {
    // Window boundaries
    if (x_ - radius_ <= 0) {
        x_ = radius_;
        speed_x_ = fabs(speed_x_); // Reflect to the right
    }
    if (x_ + radius_ >= window_width_) {
        x_ = window_width_ - radius_;
        speed_x_ = -fabs(speed_x_); // Reflect to the left
    }
    if (y_ - radius_ <= 0) {
        y_ = radius_;
        speed_y_ = fabs(speed_y_); // Reflect downwards
    }

    // Paddle collision
    SDL_FRect paddleRect = paddle_->get_rect();
    if (check_collision_circle_rect(x_, y_, radius_, paddleRect)) {
        // Calculate reflection based on where the ball hits the paddle
        float relative_intersect = (x_ - (paddleRect.x + paddleRect.w / 2.0f));
        float normalized_relative_intersect = relative_intersect / (paddleRect.w / 2.0f);
        float bounce_angle = normalized_relative_intersect * max_bounce_angle_;

        speed_x_ = speed_ * sin(bounce_angle);
        speed_y_ = -speed_ * cos(bounce_angle);

        // Adjust position to prevent sticking
        y_ = paddleRect.y - radius_ - 1;
    }

    // Brick collisions
    for (auto it = bricks_->begin(); it != bricks_->end();) {
        SDL_FRect brickRect = it->get_rect();
        if (check_collision_circle_rect(x_, y_, radius_, brickRect)) {
            // Determine side of collision
            float overlapLeft = (x_ + radius_) - brickRect.x;
            float overlapRight = (brickRect.x + brickRect.w) - (x_ - radius_);
            float overlapTop = (y_ + radius_) - brickRect.y;
            float overlapBottom = (brickRect.y + brickRect.h) - (y_ - radius_);

            bool ballFromLeft = fabs(overlapLeft) < fabs(overlapRight);
            bool ballFromTop = fabs(overlapTop) < fabs(overlapBottom);

            float minOverlapX = ballFromLeft ? overlapLeft : overlapRight;
            float minOverlapY = ballFromTop ? overlapTop : overlapBottom;

            if (fabs(minOverlapX) < fabs(minOverlapY)) {
                // Horizontal collision
                speed_x_ = ballFromLeft ? -fabs(speed_x_) : fabs(speed_x_);
            } else {
                // Vertical collision
                speed_y_ = ballFromTop ? -fabs(speed_y_) : fabs(speed_y_);
            }

            // Increase the score
            game_->increase_score(100); // Each brick is worth 100 points

            // Handle brick health
            if (it->hit()) {
                // Brick is destroyed
                it = bricks_->erase(it);
            } else {
                // Brick still exists
                ++it;
            }
        } else {
            ++it;
        }
    }
}

bool Ball::check_collision_circle_rect(float cx, float cy, float radius, const SDL_FRect& rect) {
    // Find the closest point to the circle within the rectangle
    float closestX = std::clamp(cx, rect.x, rect.x + rect.w);
    float closestY = std::clamp(cy, rect.y, rect.y + rect.h);

    // Calculate the distance between the circle's center and this closest point
    float distanceX = cx - closestX;
    float distanceY = cy - closestY;

    // If the distance is less than the circle's radius, an intersection occurs
    float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
    return distanceSquared < (radius_ * radius_);
}

bool Ball::is_off_screen() {
    return y_ - radius_ > window_height_;
}

void Ball::reset_position() {
    x_ = window_width_ / 2.0f;
    y_ = window_height_ - 60.0f;
    speed_x_ = initial_speed_x_;
    speed_y_ = -speed_;
}

void Ball::increase_speed() {
    speed_ += 50.0;
    speed_x_ *= 1.1;
    speed_y_ *= 1.1;
}