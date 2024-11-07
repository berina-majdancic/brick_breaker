#include <brick.hpp>

void Brick::render() {
  SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer_, &rect_);
}