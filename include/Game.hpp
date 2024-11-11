#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <memory>
#include "Paddle.hpp"
#include "Ball.hpp"
#include "Brick.hpp"

class Game {
public:
    Game() = default;
    void run();
    void increase_score(int amount);

private:
    void initialize();
    void handle_input();
    void update();
    void render();
    void quit();
    void initialize_bricks();

    bool running_ = false;
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    const int window_width_ = 800;
    const int window_height_ = 600;

    std::unique_ptr<Paddle> paddle_;
    std::unique_ptr<Ball> ball_;
    std::vector<Brick> bricks_;
    int score_ = 0;
    int level_ = 1;
    int lives_ = 3;
    double delta_time_ = 0.0;
    bool game_started_ = false;
    TTF_Font* font_ = nullptr;
};
