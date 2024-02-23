#pragma once
#include <string>
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

class ColliderComponent;
class AnchorComponent;

class Collision
{
public:
    static bool AABB(const SDL_FRect &rectA, const SDL_FRect &rectB);
    static bool AABB(const ColliderComponent &colA, const ColliderComponent &colB);
    static bool AABB(const ColliderComponent& col, const AnchorComponent& anch);
    static bool AABB(const AnchorComponent& anchA, const AnchorComponent& anchB);
};