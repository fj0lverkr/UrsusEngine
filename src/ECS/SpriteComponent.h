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

    bool animated = false;
    int frames = 0;
    int animationSpeed = 100;

public:
    SpriteComponent() = default;
    SpriteComponent(const char *path)
    {
        swapTexture(path);
    }

    SpriteComponent(const char *path, int nFrames, int mSpeed)
    {
        frames = nFrames;
        animationSpeed = mSpeed;
        animated = true;
        swapTexture(path);
    }

    ~SpriteComponent()
    {
        SDL_DestroyTexture(texture);
    }

    void init() override
    {
        transform = &entity->GetComponent<TransformComponent>();

        srcRect.x = srcRect.y = 0;
        srcRect.w = transform->width;
        srcRect.h = transform->height;
    }

    void update() override
    {
        if (animated)
        {
            srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks64() / animationSpeed) % frames);
        }
        destRect.x = static_cast<int>(transform->position.x);
        destRect.y = static_cast<int>(transform->position.y);
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
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