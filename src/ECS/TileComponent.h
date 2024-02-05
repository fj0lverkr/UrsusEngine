#pragma once
#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include <SDL2/SDL.h>

class TileComponent : public Component
{
private:
    int scale = 1;

public:
    SDL_Texture *texture;
    SDL_Rect srcRect, destRect;

    TileComponent() = default;

    ~TileComponent()
    {
        SDL_DestroyTexture(texture);
    }

    TileComponent(int srcX, int srcY, int posX, int posY, const char *path, int scaleFactor)
    {
        scale = scaleFactor > 1 ? scaleFactor : 1;
        texture = TextureManager::LoadTexture(path);
        srcRect.x = srcX;
        srcRect.y = srcY;
        srcRect.w = srcRect.h = 32;
        destRect.x = posX;
        destRect.y = posY;
        destRect.w = destRect.h = 32 * scale;
    }

    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
    }
};