#include <SDL_image.h>

#include <brick.hpp>
void Brick::render() {
  if (is_alive_) {
    SDL_RenderCopy(renderer_, texture_, nullptr, &rect_);
  }
}
void Brick::load_texture() {
  SDL_Surface* tempSurface = IMG_Load("assets/images/brick.png");
  if (!tempSurface) {
  }
  texture_ = SDL_CreateTextureFromSurface(renderer_, tempSurface);

  SDL_FreeSurface(tempSurface);
}
void Brick::reset() {
  health_ = initial_health_;
  is_alive_ = true;
}