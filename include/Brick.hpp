#pragma once
#include <SDL.h>

class Brick {
public:
    Brick(SDL_Renderer* renderer, float x, float y, float width, float height, int health)
        : renderer_(renderer), health_(health) {
        rect_ = { x, y, width, height };
        update_color();
    }

    void render();
    SDL_FRect get_rect() const { return rect_; }
    bool hit(); // Returns true if brick is destroyed

private:
    void update_color();

    SDL_Renderer* renderer_;
    SDL_FRect rect_;
    int health_;
    SDL_Color color_;
};
