#include "Collision.hpp"
#include "ECS/ColliderComponent.hpp"
#include "ECS/AnchorComponent.hpp"

bool Collision::LineCollision(SDL_FPoint& pointA, SDL_FPoint& pointB, const SDL_FRect& rect)
{
    return SDL_IntersectFRectAndLine(&rect, &pointA.x, &pointA.y, &pointB.x, &pointB.y);
}

bool Collision::LineCollision(const SDL_FPoint& pointA, const SDL_FPoint& pointB, const SDL_FPoint& pointC, const SDL_FPoint& pointD)
{

    return false;
}

bool Collision::AABB(const SDL_FRect& rectA, const SDL_FRect& rectB)
{
    return SDL_HasIntersectionF(&rectA, &rectB);
}

bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB)
{
    if (AABB(colA.collider, colB.collider) && colA.uuid != colB.uuid && colA.getType() == ColliderType::AABB && colB.getType() == ColliderType::AABB)
    {
        return true;
    }
    return false;
}

bool Collision::AABB(const ColliderComponent& col, const AnchorComponent& anch)
{
    if (AABB(col.collider, anch.collider) && col.uuid != anch.uuid && col.getType() == ColliderType::AABB)
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

bool Collision::PointCollision(const ColliderComponent& colA, const ColliderComponent& colB)
{
    if ((colA.getType() == ColliderType::Polygon && colB.getType() == ColliderType::AABB) ||
        (colB.getType() == ColliderType::Polygon && colA.getType() == ColliderType::AABB))
    {
        // Test for collision between each line (pair of points) of the Polygon's edge (either colA or ColB) and the other's Rectangle (either colA or ColB).
        return false;
    }
    else if (colA.getType() == ColliderType::Polygon && colB.getType() == ColliderType::Polygon)
    {
        // Test each line of colA against each line of colB. More research needed.
        return false;
    }
    else
    {
        return false;
    }
}

bool Collision::PointCollision(const ColliderComponent& col, const AnchorComponent& anch)
{
    if (col.getType() == ColliderType::Polygon)
    {
        // Test for collision between each line (pair of points) of the Polygon's edge and the achor's Rectangle.
        int pointCount = static_cast<int>(col.colliderPoints.size());
        ColliderComponent& c = const_cast<ColliderComponent&>(col);
        for(int i = 0; i < pointCount; i++)
        {
            int nextPointIndex = i + 1 >= pointCount ? 0 : i + 1;
            if (LineCollision(c.colliderPoints[i], c.colliderPoints[nextPointIndex], anch.collider))
            {
                return true;
            }
        }
    }
    return false;
}
