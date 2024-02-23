#pragma once

#include "Components.hpp"
#include <SDL2/SDL.h>
#include "Game.hpp"

class PolygonLinesComponent : public Component

    /*
    This component can be used to visualize the polygon shaped objects from a Tiled tilesheet objectgroup.
    It uses the point provided by the tilesheet data to draw lines, as SDL2 is not capable of drawing more complex shapes (other than triangles with SDL_RenderGeometry)
    this is the most sensible way of doing it.
    */

{
private:
    PolygonTransformComponent* transform;
    SDL_FPoint* destPoints;

public:
    PolygonLinesComponent()
    {
        this->transform = nullptr;
        this->destPoints = nullptr;
    }

    ~PolygonLinesComponent()
    {
    }

    void init() override
    {
        transform = &entity->GetComponent<PolygonTransformComponent>();
        destPoints = new SDL_FPoint[transform->pointCount];
        for (int i = 0; i < transform->pointCount; i++)
        {
            destPoints[i] = { transform->points[i].x, transform->points[i].y};
        }
    }

    void update() override
    {
        for (int i = 0; i < transform->pointCount; i++)
        {
            destPoints[i].x = transform->points[i].x - Game::camera.GetViewFinder().x;
            destPoints[i].y = transform->points[i].y - Game::camera.GetViewFinder().y;
        }
    }

    void draw() override
    {
        SDL_RenderDrawLinesF(Game::renderer, destPoints, transform->pointCount);
    }
};