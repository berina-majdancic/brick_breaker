#include <SDL_image.h>

#include <brick.hpp>
SDL_Texture* Brick::texture_ = nullptr;
void Brick::render() {
  if (is_alive_) {
    SDL_RenderCopy(renderer_, texture_, nullptr, &rect_);
  }
}
void Brick::load_texture(SDL_Renderer* renderer) {
  SDL_Surface* tempSurface = IMG_Load("assets/images/brick.png");
  if (!tempSurface) {
  }
  texture_ = SDL_CreateTextureFromSurface(renderer, tempSurface);

  SDL_FreeSurface(tempSurface);
}
void Brick::reset() {
  health_ = initial_health_;
  is_alive_ = true;
}
