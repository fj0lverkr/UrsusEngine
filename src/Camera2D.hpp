#pragma once

#include <SDL2/SDL.h>
#include "Vector2D.hpp"

class Camera2D
{
private:
    SDL_FRect viewFinder;

public:
    Camera2D();
    Camera2D(float x, float y, int w, int h);
    SDL_FRect GetViewFinder() const;
    void follow(Vector2D &position, int windowWidth, int windowHeight);
};