#pragma once
#include "../Game.h"
#include "Components.h"
#include <set>

class KeyboardController : public Component
{
private:
    std::set<SDL_Keycode> pressedKeys;

    bool isMovementKey(SDL_Keycode refKeycode)
    {
        return refKeycode == SDLK_w || refKeycode == SDLK_a || refKeycode == SDLK_s || refKeycode == SDLK_d;
    }

    void doMovement()
    {
        transform->velocity.Zero();
        for (auto key : pressedKeys)
        {
            if (isMovementKey(key))
            {
                switch (key)
                {
                case SDLK_w:
                    transform->velocity.y = -1;
                    sprite->Play("WalkSide");
                    break;
                case SDLK_a:
                    transform->velocity.x = -1;
                    sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                    sprite->Play("WalkSide");
                    break;
                case SDLK_s:
                    transform->velocity.y = 1;
                    sprite->Play("WalkSide");
                    break;
                case SDLK_d:
                    transform->velocity.x = 1;
                    sprite->spriteFlip = SDL_FLIP_NONE;
                    sprite->Play("WalkSide");
                    break;
                default:
                    break;
                }
            }
        }
        transform->velocity.Normalize();
    }

public:
    TransformComponent *transform;
    SpriteComponent *sprite;

    void init() override
    {
        transform = &entity->GetComponent<TransformComponent>();
        sprite = &entity->GetComponent<SpriteComponent>();
    }

    void update() override
    {
        if (Game::event.type == SDL_KEYDOWN)
        {
            pressedKeys.emplace(Game::event.key.keysym.sym); // elements in a set are always unique, so we can just add it.

            // Movement keys are handled differently, all other keys go here:
            switch (Game::event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                break;
            default:
                break;
            }
        }

        if (Game::event.type == SDL_KEYUP)
        {
            // remove the lifted key from the set of pressed keys
            pressedKeys.erase(pressedKeys.find(Game::event.key.keysym.sym));
        }

        // Handle Movement
        doMovement();

        // Reset sprite
        if (transform->velocity.IsZero())
        {
            sprite->Play("Idle");
        }
    }
};