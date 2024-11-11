#pragma once
#include <SDL.h>
#include <vector>
#include "Paddle.hpp"
#include "Brick.hpp"

// Forward declaration
class Game;

class Ball {
public:
    Ball(int window_width, int window_height, Paddle* paddle, std::vector<Brick>* bricks, SDL_Renderer* renderer, Game* game)
        : window_width_(window_width), window_height_(window_height), paddle_(paddle), bricks_(bricks), renderer_(renderer), game_(game) {
        radius_ = 8;
        reset_position();
        speed_ = 300.0; // Pixels per second
        initial_speed_x_ = speed_ * 0.5;
        speed_x_ = initial_speed_x_;
        speed_y_ = -speed_;
        max_bounce_angle_ = 75 * M_PI / 180; // Convert degrees to radians
    }

    void render();
    void move(double delta_time);
    bool is_off_screen();
    void reset_position();
    void increase_speed();

private:
    void handle_collisions();
    bool check_collision_circle_rect(float cx, float cy, float radius, const SDL_FRect& rect);

    float x_, y_;
    float radius_;
    double speed_, speed_x_, speed_y_;
    double initial_speed_x_;
    double max_bounce_angle_;
    int window_width_, window_height_;
    Paddle* paddle_;
    std::vector<Brick>* bricks_;
    SDL_Renderer* renderer_;
    Game* game_;

    SDL_FRect get_rect() const {
        return { x_ - radius_, y_ - radius_, radius_ * 2, radius_ * 2 };
    }
};
