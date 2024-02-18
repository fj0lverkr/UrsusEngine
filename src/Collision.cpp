#include "Collision.hpp"
#include "ECS/ColliderComponent.hpp"
#include "ECS/AnchorComponent.hpp"

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
        return true;
    }
    return false;
}

bool Collision::AABB(const ColliderComponent& col, const AnchorComponent& anch)
{
    if (AABB(col.collider, anch.collider) && col.uuid != anch.uuid)
    {
        return true;
    }
    return false;
}

bool Collision::AABB(const AnchorComponent& anchA, const AnchorComponent& anchB)
{
    if (AABB(anchA.collider, anchB.collider) && anchA.uuid != anchB.uuid)
    {
        return true;
    }
    return false;
}
