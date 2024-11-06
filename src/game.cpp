#include <ball.hpp>
#include <game.hpp>
#include <paddle.hpp>

void Game::create_window() {
  window_ = SDL_CreateWindow("My SDL Window", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, window_width,
                             window_height, SDL_WINDOW_SHOWN);
  if (window_ == nullptr) {
    std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError()
              << std::endl;
    SDL_Quit();
  }
}
void Game::renderer_init() {
  renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
}
void Game::start() {
  create_window();
  renderer_init();
  running_ = true;
  SDL_Event event;

  Paddle paddle(window_width_, window_height_);
  Ball ball(window_width_, window_height_);

  while (running_) {  // game loop

    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);

    paddle.render(renderer_);
    ball.render(renderer_);
    ball.move(paddle);

    // Swap the buffers
    SDL_RenderPresent(renderer_);
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        running_ = false;
      else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_LEFT:
            paddle.move(Direction::left);

            break;
          case SDLK_RIGHT:
            paddle.move(Direction::right);
            break;
        }
      }
    }
  }

  SDL_DestroyWindow(window_);
  SDL_Quit();
}
