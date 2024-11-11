#pragma once
#include <SDL.h>

class Paddle {
public:
    Paddle(int window_width, int window_height, SDL_Renderer* renderer)
        : window_width_(window_width), window_height_(window_height), renderer_(renderer) {
        width_ = 100;
        height_ = 20;
        x_ = (window_width_ - width_) / 2;
        y_ = window_height_ - height_ - 30;
        rect_ = { static_cast<float>(x_), static_cast<float>(y_), static_cast<float>(width_), static_cast<float>(height_) };
        speed_ = 400.0; // Pixels per second
    }

    void render();
    void move(const Uint8* keystate, double delta_time, int mouse_x = -1);
    void reset_position();

    float get_x() const { return x_; }
    float get_y() const { return y_; }
    float get_width() const { return width_; }
    float get_height() const { return height_; }
    SDL_FRect get_rect() const { return rect_; }

private:
    int window_width_, window_height_;
    float x_, y_;
    float width_, height_;
    double speed_;
    SDL_Renderer* renderer_;
    SDL_FRect rect_;
};
