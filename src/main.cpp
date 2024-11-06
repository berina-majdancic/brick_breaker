#include <SDL2/SDL.h>
#include <math.h>

#include <iostream>
#include <paddle.hpp>

#include "../include/ball.hpp"
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
int main() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
              << std::endl;
    return 1;
  }

  SDL_Window* window = SDL_CreateWindow("My SDL Window", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                                        WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError()
              << std::endl;
    SDL_Quit();
    return 1;
  }
  bool running = true;
  SDL_Renderer* renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  Paddle paddle(WINDOW_WIDTH, WINDOW_HEIGHT);
  Ball ball(WINDOW_WIDTH, WINDOW_HEIGHT);
  while (running) {
    SDL_Event event;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    paddle.render(renderer);
    ball.render(renderer);
    ball.move(paddle);

    // Swap the buffers
    SDL_RenderPresent(renderer);
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) running = false;
      if (event.type == SDL_KEYDOWN) {
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

  SDL_DestroyWindow(window);
  SDL_Quit();
}