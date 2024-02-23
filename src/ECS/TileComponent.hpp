#pragma once
#include "ECS.hpp"
#include "TransformComponent.hpp"
#include "SpriteComponent.hpp"
#include <SDL2/SDL.h>

class TileComponent : public Component
{
private:
    int scale = 1;

public:
    SDL_Texture *texture;
    SDL_Rect srcRect;
    SDL_FRect destRect;
    Vector2D position;

    TileComponent() = default;

    ~TileComponent()
    {
        SDL_DestroyTexture(texture);
    }

    TileComponent(int srcX, int srcY, float posX, float posY, const char *path, int tileSize, int scaleFactor)
    {
        scale = scaleFactor > 1 ? scaleFactor : 1;
        texture = TextureManager::LoadTexture(path);
        srcRect.x = srcX;
        srcRect.y = srcY;
        srcRect.w = srcRect.h = tileSize;
        destRect.x = posX;
        destRect.y = posY;
        destRect.w = destRect.h = static_cast<float>(tileSize * scale);
        position.x = posX;
        position.y = posY;
    }

    void update() override
    {
        destRect.x = position.x - Game::camera.GetViewFinder().x;
        destRect.y = position.y - Game::camera.GetViewFinder().y;
    }

    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
    }
};