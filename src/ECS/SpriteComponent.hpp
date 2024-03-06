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
    int frames = 0;
    int animationSpeed = 100;

public:
    int animationIndex = 0;
    std::map<const char *, Animation> animations;
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    SpriteComponent() = default;

    SpriteComponent(std::string assetId, bool isPlayer)
    {
        swapTexture(assetId);
    }

    SpriteComponent(std::string assetId, std::map<const char*, Animation> animations, const char* idleAnimation)
    {
        animated = true;

        this->animations = animations;

        Play(idleAnimation);

        swapTexture(assetId);
    }

    ~SpriteComponent()
    {
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

    void swapTexture(std::string assetId)
    {
        texture = Game::assets->GetTexture(assetId);
    }

    void Play(const char *animName)
    {
        frames = animations[animName].frames;
        animationIndex = animations[animName].index;
        animationSpeed = animations[animName].animationSpeed;
    }
};