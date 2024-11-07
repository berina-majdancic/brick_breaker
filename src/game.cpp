#include <ball.hpp>
#include <game.hpp>
#include <paddle.hpp>

void Game::initialize() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
              << std::endl;
  }
  window_ = SDL_CreateWindow("My SDL Window", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, window_width_,
                             window_height_, SDL_WINDOW_SHOWN);
  if (window_ == nullptr) {
    std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError()
              << std::endl;
    SDL_Quit();
  }
  renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
  paddle_ = Paddle(window_width_, window_height_, renderer_);
  ball_ = Ball(window_width_, window_height_, paddle_, renderer_);
}

void Game::run() {
  initialize();
  Uint64 current_time, last_time = SDL_GetPerformanceCounter();
  delta_time_ = 0;

  while (running_) {  // game loop
    current_time = SDL_GetPerformanceCounter();
    delta_time_ = (double)((current_time - last_time) * 1000 /
                           (double)SDL_GetPerformanceFrequency());

    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);

    paddle_.render();
    ball_.render();
    ball_.move(delta_time_);

    // Swap the buffers
    SDL_RenderPresent(renderer_);
    handle_input();
    last_time = current_time;
  }

  SDL_DestroyWindow(window_);
  SDL_Quit();
}
void Game::handle_input() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT)
      running_ = false;
    else if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
        case SDLK_LEFT:
          paddle_.move(Direction::left, delta_time_);

          break;
        case SDLK_RIGHT:
          paddle_.move(Direction::right, delta_time_);
          break;
      }
    }
  }
}