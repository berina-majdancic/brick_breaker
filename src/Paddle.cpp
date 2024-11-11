#include "Paddle.hpp"

void Paddle::render() {
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
    SDL_RenderFillRectF(renderer_, &rect_);
}

void Paddle::move(const Uint8* keystate, double delta_time, int mouse_x) {
    if (mouse_x >= 0) {
        // Move paddle based on mouse position
        x_ = mouse_x - width_ / 2.0f;
        // Clamp within window boundaries
        if (x_ < 0) x_ = 0;
        if (x_ + width_ > window_width_) x_ = window_width_ - width_;
    } else {
        // Existing keyboard input handling
        if (keystate[SDL_SCANCODE_LEFT]) {
            x_ -= speed_ * delta_time;
            if (x_ < 0) x_ = 0;
        }
        if (keystate[SDL_SCANCODE_RIGHT]) {
            x_ += speed_ * delta_time;
            if (x_ + width_ > window_width_) x_ = window_width_ - width_;
        }
    }
    rect_.x = static_cast<float>(x_);
}

void Paddle::reset_position() {
    x_ = (window_width_ - width_) / 2.0f;
    rect_.x = x_;
}
