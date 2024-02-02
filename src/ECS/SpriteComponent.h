#pragma once

#include "Components.h"
#include "../TextureManager.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class SpriteComponent : public Component
{
private:
    TransformComponent *transform;
    SDL_Texture *texture;
    SDL_Rect srcRect, destRect;

public:
    SpriteComponent() = default;
    SpriteComponent(const char *path)
    {
        swapTexture(path);
    }

    void init() override
    {
        transform = &entity->GetComponent<TransformComponent>();

        srcRect.x = srcRect.y = 0;
        srcRect.w = srcRect.h = 32;
        destRect.w = destRect.h = 64;
    }

    void update() override
    {
        destRect.x = transform->position.x;
        destRect.y = transform->position.y;
    }

    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect);
    }

    void swapTexture(const char *path)
    {
        texture = TextureManager::LoadTexture(path);
    }
};