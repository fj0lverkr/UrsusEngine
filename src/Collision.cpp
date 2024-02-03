#include "Collision.h"
#include "ECS/ColliderComponent.h"

bool Collision::AABB(const SDL_Rect &rectA, const SDL_Rect &rectB)
{
    return rectA.x + rectA.w >= rectB.x &&
           rectB.x + rectB.w >= rectA.x &&
           rectA.y + rectA.h >= rectB.y &&
           rectB.y + rectB.h >= rectA.y;
}

bool Collision::AABB(const ColliderComponent &colA, const ColliderComponent &colB)
{
    if (AABB(colA.collider, colB.collider) && colA.uuid != colB.uuid)
    {
        std::cout << colA.tag << " collided with " << colB.tag << std::endl;
        return true;
    }
    return false;
}
