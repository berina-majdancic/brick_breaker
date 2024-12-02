#include <SDL_image.h>

#include <brick.hpp>
SDL_Texture* Brick::texture_ = nullptr;
bool Brick::render() {
  if (is_alive()) {
    SDL_RenderCopy(renderer_, texture_, nullptr, &rect_);
    return 1;
  }
  return 0;
}
void Brick::load_texture(SDL_Renderer* renderer) {
  SDL_Surface* tempSurface = IMG_Load("assets/images/brick.png");
  if (!tempSurface) {
  }
  texture_ = SDL_CreateTextureFromSurface(renderer, tempSurface);

  SDL_FreeSurface(tempSurface);
}
void Brick::reset() { health_ = 0; }
void Brick::damage() { health_--; }