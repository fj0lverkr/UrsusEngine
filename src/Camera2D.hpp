#pragma once

#include <SDL2/SDL.h>
#include "Vector2D.hpp"

class Camera2D
{
private:
    SDL_Rect viewFinder;

public:
    Camera2D();
    Camera2D(int x, int y, int w, int h);
    SDL_Rect GetViewFinder();
    void follow(Vector2D &position, int windowWidth, int windowHeight);
};