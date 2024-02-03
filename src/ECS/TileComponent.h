#pragma once
#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include <SDL2/SDL.h>

struct TileComponent : public Component
{
    TransformComponent *transform;
    SpriteComponent *sprite;

    SDL_Rect tileRect;
    int tileID;
    char *path;

    TileComponent() = default;
    TileComponent(int x, int y, int w, int h, int id)
    {
        tileRect.x = x;
        tileRect.y = y;
        tileRect.w = w;
        tileRect.h = h;

        tileID = id;

        switch (tileID)
        {
        case 0:
            path = "assets/map/water.png";
            break;
        case 1:
            path = "assets/map/dirt.png";
            break;
        case 2:
            path = "assets/map/grass.png";
            break;
        default:
            break;
        }
    }

    void init() override
    {
        if (!entity->hasComponent<TransformComponent>())
        {
            entity->addComponent<TransformComponent>(tileRect.x, tileRect.y, tileRect.w, tileRect.h, 1);
        }
        transform = &entity->GetComponent<TransformComponent>();

        if (!entity->hasComponent<SpriteComponent>())
        {
            entity->addComponent<SpriteComponent>(path);
        }
        sprite = &entity->GetComponent<SpriteComponent>();
    }
};