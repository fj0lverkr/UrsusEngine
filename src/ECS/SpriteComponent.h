#pragma once

#include "Components.h"
#include "Animation.h"
#include "../TextureManager.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <map>

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
    int animationIndex = 0;
    std::map<const char *, Animation> animations;
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    SpriteComponent() = default;
    SpriteComponent(const char *path)
    {
        swapTexture(path);
    }

    SpriteComponent(const char *path, bool isAnimated)
    {
        animated = isAnimated;

        Animation idle = Animation(0, 9, 100);
        Animation walk_side = Animation(1, 7, 100);
        Animation walk_front = Animation(2, 7, 100);

        animations.emplace("Idle", idle);
        animations.emplace("WalkSide", walk_side);
        animations.emplace("WalkFront", walk_front);

        Play("Idle");

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

        srcRect.y = animationIndex * transform->height;

        destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
        destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
    }

    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
    }

    void swapTexture(const char *path)
    {
        texture = TextureManager::LoadTexture(path);
    }

    void Play(const char *animName)
    {
        frames = animations[animName].frames;
        animationIndex = animations[animName].index;
        animationSpeed = animations[animName].animationSpeed;
    }
};