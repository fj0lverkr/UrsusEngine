#pragma once

#include "Components.hpp"
#include "Animation.hpp"
#include "../TextureManager.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <map>

class SpriteComponent : public Component
{
private:
    TransformComponent *transform;
    SDL_Texture *texture;
    SDL_Rect srcRect;
    SDL_FRect destRect;

    bool animated = false;
    bool isPlayer = false;
    int frames = 0;
    int animationSpeed = 100;

public:
    int animationIndex = 0;
    std::map<const char *, Animation> animations;
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    SpriteComponent() = default;
    SpriteComponent(const char *path, bool isPlayer)
    {
        swapTexture(path);
        this->isPlayer = isPlayer;
    }

    SpriteComponent(const char *path, bool isAnimated,bool isPlayer)
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

    ~SpriteComponent()
    {
        SDL_DestroyTexture(texture);
    }

    void init() override
    {
        transform = &entity->GetComponent<TransformComponent>();

        srcRect.x = srcRect.y = 0;
        srcRect.w = static_cast<int>(transform->width);
        srcRect.h = static_cast<int>(transform->height);
    }

    void update() override
    {
        if (animated)
        {
            srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks64() / animationSpeed) % frames);
        }

        srcRect.y = static_cast<int>(animationIndex * transform->height);

        destRect.x = transform->position.x - Game::camera.GetViewFinder().x;
        destRect.y = transform->position.y - Game::camera.GetViewFinder().y;
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