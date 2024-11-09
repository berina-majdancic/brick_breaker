#include <brick.hpp>

void Brick::render() {
  if (health_ > 0) {
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer_, &rect_);
  }
}