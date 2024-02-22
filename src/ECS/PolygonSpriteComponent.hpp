#pragma once

#include "Components.hpp"
#include "Animation.hpp"
#include "../TextureManager.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <map>
//temp include to get temp draw to work
#include "Game.hpp"

class PolygonSpriteComponent : public Component
{
private:
    PolygonTransformComponent* transform;
    SDL_Texture* texture;
    SDL_Rect srcRect;
    SDL_Point* destPoints;

    bool animated = false;
    bool isPlayer = false;
    int frames = 0;
    int animationSpeed = 100;

public:
    int animationIndex = 0;
    std::map<const char*, Animation> animations;
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    PolygonSpriteComponent() = default;
    PolygonSpriteComponent(const char* path, bool isPlayer)
    {
        swapTexture(path);
        this->isPlayer = isPlayer;
    }

    PolygonSpriteComponent(const char* path, bool isAnimated, bool isPlayer)
    {
        this->isPlayer = isPlayer;
        animated = isAnimated;

        // TODO this is now very specific for our player, we should rework this...
        Animation idle = Animation(0, 9, 100);
        Animation walk_side = Animation(1, 7, 100);
        Animation walk_front = Animation(2, 7, 100);

        animations.emplace("Idle", idle);
        animations.emplace("WalkSide", walk_side);
        animations.emplace("WalkFront", walk_front);

        Play("Idle");

        swapTexture(path);
    }

    ~PolygonSpriteComponent()
    {
        SDL_DestroyTexture(texture);
    }

    void init() override
    {
        transform = &entity->GetComponent<PolygonTransformComponent>();
        destPoints = new SDL_Point [transform->pointCount];
        for (int i = 0; i < transform->pointCount; i++)
        {
            destPoints[i] = transform->points[i];
        }
        SDL_Point textureSize = {0, 0};
        SDL_QueryTexture(texture, NULL, NULL, &textureSize.x, &textureSize.y);
        srcRect.x = srcRect.y = 0;
        srcRect.w = textureSize.x;
        srcRect.h = textureSize.y;
    }

    void update() override
    {
        if (animated)
        {
            srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks64() / animationSpeed) % frames);
        }

        srcRect.y = animationIndex * srcRect.h;
        for (int i = 0; i < transform->pointCount; i++)
        {
            destPoints[i].x = transform->points[i].x - Game::camera.GetViewFinder().x;
            destPoints[i].y = transform->points[i].y - Game::camera.GetViewFinder().y;
        }
    }

    void draw() override
    {
        //TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
        // for drawing, a lot can be simplified if we use SDL_RenderDrawGeometry instead...
        SDL_RenderDrawLines(Game::renderer, destPoints, transform->pointCount);
    }

    void swapTexture(const char* path)
    {
        texture = TextureManager::LoadTexture(path);
    }

    void Play(const char* animName)
    {
        frames = animations[animName].frames;
        animationIndex = animations[animName].index;
        animationSpeed = animations[animName].animationSpeed;
    }
};