#include "Brick.hpp"

void Brick::render() {
    SDL_SetRenderDrawColor(renderer_, color_.r, color_.g, color_.b, 255);
    SDL_RenderFillRectF(renderer_, &rect_);
}

bool Brick::hit() {
    health_--;
    if (health_ <= 0) {
        return true; // Brick is destroyed
    } else {
        update_color();
        return false; // Brick still exists
    }
}

void Brick::update_color() {
    // Update the color based on the new health
    int max_health = 5; // Assuming max health is 5
    int intensity = 255 * health_ / max_health;

    // Clamp intensity to the range [0, 255]
    if (intensity > 255) intensity = 255;
    if (intensity < 0) intensity = 0;

    // Explicitly cast each component to Uint8
    color_ = SDL_Color{
        static_cast<Uint8>(intensity),
        static_cast<Uint8>(intensity / 2),
        0,
        255 // Alpha value
    };
}