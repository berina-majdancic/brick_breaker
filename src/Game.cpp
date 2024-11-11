#include "Game.hpp"
#include <iostream>
#include <string>

void Game::run() {
    initialize();

    Uint64 current_time, last_time = SDL_GetPerformanceCounter();
    delta_time_ = 0;

    while (running_) {
        current_time = SDL_GetPerformanceCounter();
        delta_time_ = (current_time - last_time) / static_cast<double>(SDL_GetPerformanceFrequency());

        handle_input();
        update();
        render();

        last_time = current_time;
    }
    quit();
}

void Game::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cerr << "SDL Initialization Failed: " << SDL_GetError() << std::endl;
        exit(1);
    }

    window_ = SDL_CreateWindow("Brick Breaker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               window_width_, window_height_, SDL_WINDOW_SHOWN);

    if (!window_) {
        std::cerr << "Window Creation Failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer_) {
        std::cerr << "Renderer Creation Failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window_);
        SDL_Quit();
        exit(1);
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "TTF Initialization Failed: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
        exit(1);
    }

    // Load a font
    font_ = TTF_OpenFont("font/PressStart2P-Regular.ttf", 24);
    if (!font_) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        TTF_Quit();
        SDL_DestroyRenderer(renderer_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
        exit(1);
    }

    // Initialize game objects
    paddle_ = std::make_unique<Paddle>(window_width_, window_height_, renderer_);
    initialize_bricks();
    ball_ = std::make_unique<Ball>(window_width_, window_height_, paddle_.get(), &bricks_, renderer_, this);
    score_ = 0;
    level_ = 1;
    lives_ = 3;
    running_ = true;
    game_started_ = false;
}

void Game::initialize_bricks() {
    bricks_.clear();
    int rows = 5 + level_; // Increase rows with each level
    int cols = 10;
    int brick_width = (window_width_ - (cols + 1) * 5) / cols;
    int brick_height = 20;

    // Generate a random pattern
    srand(SDL_GetTicks()); // Seed the random number generator

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Randomly decide whether to place a brick
            if (rand() % 2 == 0) {
                int x = 5 + j * (brick_width + 5);
                int y = 50 + i * (brick_height + 5);
                int health = 1 + level_ / 2;
                bricks_.emplace_back(renderer_, x, y, brick_width, brick_height, health);
            }
        }
    }

    // Ensure there is at least one brick
    if (bricks_.empty()) {
        int x = (window_width_ - brick_width) / 2;
        int y = 50;
        bricks_.emplace_back(renderer_, x, y, brick_width, brick_height, 1 + level_ / 2);
    }
}

void Game::handle_input() {
    SDL_Event event;
    int mouse_x = -1;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            running_ = false;

        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                running_ = false;
            }
            if (event.key.keysym.sym == SDLK_SPACE) {
                game_started_ = true;
            }
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                game_started_ = true;
            }
        }
    }

    // Get mouse state
    int x, y;
    SDL_GetMouseState(&x, &y);
    mouse_x = x; // Always track mouse position

    const Uint8* keystate = SDL_GetKeyboardState(nullptr);
    paddle_->move(keystate, delta_time_, mouse_x);
}

void Game::update() {
    if (game_started_) {
        ball_->move(delta_time_);

        // Check for ball going out of bounds
        if (ball_->is_off_screen()) {
            lives_--;
            if (lives_ == 0) {
                running_ = false;
            } else {
                // Reset ball and paddle positions
                paddle_->reset_position();
                ball_->reset_position();
                game_started_ = false; // Wait for user to start again
            }
        }

        // Check for level completion
        if (bricks_.empty()) {
            level_++;
            initialize_bricks();
            ball_->increase_speed();
            game_started_ = false; // Wait for user to start next level
        }
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);

    // Render game objects
    paddle_->render();
    ball_->render();

    for (auto& brick : bricks_) {
        brick.render();
    }

    // Render HUD (Score, Lives, Level)
    SDL_Color textColor = {255, 255, 255, 255};
    std::string hudText = "Score: " + std::to_string(score_) + "  Lives: " + std::to_string(lives_) + "  Level: " + std::to_string(level_);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font_, hudText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer_, textSurface);
    SDL_Rect textRect = {10, 10, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer_, textTexture, nullptr, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    // If game is not started, display a "Press Space or Click to Start" message
    if (!game_started_) {
        std::string startText = "Press Space or Click to Start";
        textSurface = TTF_RenderText_Solid(font_, startText.c_str(), textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer_, textSurface);
        textRect = { (window_width_ - textSurface->w) / 2, window_height_ / 2 - textSurface->h, textSurface->w, textSurface->h };
        SDL_RenderCopy(renderer_, textTexture, nullptr, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }

    SDL_RenderPresent(renderer_);
}

void Game::quit() {
    TTF_CloseFont(font_);
    TTF_Quit();
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void Game::increase_score(int amount) {
    score_ += amount;
}
