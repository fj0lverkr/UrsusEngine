#pragma once

#include <SDL2/SDL.h>
#include "ECS/ECS.h"

class Camera2D
{
private:
    SDL_Rect viewFinder;

public:
    Camera2D(int x, int y, int w, int h);
};