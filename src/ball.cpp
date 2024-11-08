#include <ball.hpp>

void Ball::render(SDL_Renderer* renderer) {
  const int diameter = (radius_ * 2);

  int x = (radius_ - 1);
  int y = 0;
  int tx = 1;
  int ty = 1;
  int error = (tx - diameter);

  while (x >= y) {
    for (int i = centre_y_ - y; i <= centre_y_ + y; i++) {
      SDL_RenderDrawPoint(renderer, centre_x_ + x,
                          i);  // Right side of the circle
      SDL_RenderDrawPoint(renderer, centre_x_ - x,
                          i);  // Left side of the circle
    }

    if (x != y) {
      for (int i = centre_y_ - x; i <= centre_y_ + x; i++) {
        SDL_RenderDrawPoint(renderer, centre_x_ + y,
                            i);  // Right side of the circle
        SDL_RenderDrawPoint(renderer, centre_x_ - y,
                            i);  // Left side of the circle
      }
    }

    if (error <= 0) {
      ++y;
      error += ty;
      ty += 2;
    }

    if (error > 0) {
      --x;
      tx += 2;
      error += (tx - diameter);
    }
  }
}
