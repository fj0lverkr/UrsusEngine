#pragma once
#include "SDL2/SDL.h"

/*
Collision Types:
    Axis-Aligned Bounding Box Collider (AABB)
        Given two colliders A and B, we can assume a collision (touching or overlapping of colliders) when:
        A.x + A.w >= B.x &&
        B.x + B.w >= A.x &&
        A.y + A.h >= B.y &&
        B.y + B.h >= A.y
*/

class Collision
{
public:
    static bool AABB(const SDL_Rect &rectA, const SDL_Rect &rectB);
};