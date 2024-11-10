#include <cstdlib>
#include <ctime>
#include <game.hpp>
#include <iostream>
void Game::run() {
  initialize();
  Uint64 current_time, last_time = SDL_GetPerformanceCounter();
  delta_time_ = 0;

  while (running_) {
    current_time = SDL_GetPerformanceCounter();
    delta_time_ = calculate_delta_time(current_time, last_time);

    handle_input();
    render();

    SDL_RenderPresent(renderer_);  // Swap the buffers
    last_time = current_time;
  }
  quit();
}
void Game::initialize() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
              << std::endl;
    return;
  }
  window_ = SDL_CreateWindow("My SDL Window", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, window_width_,
                             window_height_, SDL_WINDOW_SHOWN);
  if (window_ == nullptr) {
    std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError()
              << std::endl;
    SDL_Quit();
    return;
  }
  renderer_ = SDL_CreateRenderer(  // add error handling
      window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  paddle_ = Paddle(window_width_, window_height_, renderer_);
  initialize_bricks();
  ball_ = Ball(window_width_, window_height_, &paddle_, &brick_, renderer_);
  running_ = true;
}
double Game::calculate_delta_time(Uint64 current_time, Uint64 last_time) {
  double delta_time = (double)((current_time - last_time) * 1000 /
                               (double)SDL_GetPerformanceFrequency());
  if (delta_time < 16.67) {
    SDL_Delay(static_cast<Uint32>(16.67 - delta_time));
  }
  delta_time = (double)((current_time - last_time) * 1000 /
                        (double)SDL_GetPerformanceFrequency());
  return delta_time;
}
void Game::render() {
  SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
  SDL_RenderClear(renderer_);
  for (int i = 0; i < NUM_OF_BRICKS; i++) {
    brick_[i].render();
  }
  paddle_.render();
  ball_.render();
  ball_.move(delta_time_);
}
void Game::handle_input() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) running_ = false;
  }
  const Uint8* keystate = SDL_GetKeyboardState(nullptr);
  if (keystate[SDL_SCANCODE_LEFT]) {
    paddle_.move(Direction::left, delta_time_);
  }
  if (keystate[SDL_SCANCODE_RIGHT]) {
    paddle_.move(Direction::right, delta_time_);
  }
}
void Game::quit() {
  SDL_DestroyWindow(window_);
  SDL_Quit();
}
void Game::initialize_bricks() {
  srand(static_cast<unsigned>(time(0)));
  int x = 5, y = 5;
  int health = 1;
  for (int i = 0; i < NUM_OF_BRICKS; i++) {
    /*int random_x = (rand() % (window_width_ - 70));
    int random_y = (rand() % (window_height_ / 2)) + 5;*/

    brick_[i] = Brick(renderer_, x, y, health);
    x += 75;

    if (x + 60 >= window_width_) {
      x = 5;
      y += 35;
    }
    if (i == NUM_OF_BRICKS - 20) health = 4;
  }
}