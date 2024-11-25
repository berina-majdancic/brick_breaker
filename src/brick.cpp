#include <SDL_image.h>

#include <brick.hpp>
void Brick::render() {
  if (health_ > 0) {
    SDL_RenderCopy(renderer_, texture_, nullptr, &rect_);
  }
}
void Brick::load_texture() {
  SDL_Surface* tempSurface = IMG_Load("../assets/images/brick.png");
  if (!tempSurface) {
  }
  texture_ = SDL_CreateTextureFromSurface(renderer_, tempSurface);
  SDL_FreeSurface(tempSurface);
}