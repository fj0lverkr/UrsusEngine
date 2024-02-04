#pragma once
#include "../Game.h"
#include "Components.h"

struct KeyboardController : public Component
{
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
            switch (Game::event.key.keysym.sym)
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
            case SDLK_ESCAPE:
                break;
            default:
                break;
            }
        }

        if (Game::event.type == SDL_KEYUP)
        {
            switch (Game::event.key.keysym.sym)
            {
            case SDLK_w:
                transform->velocity.y = 0;
                sprite->Play("Idle");
                break;
            case SDLK_a:
                transform->velocity.x = 0;
                sprite->Play("Idle");
                break;
            case SDLK_s:
                transform->velocity.y = 0;
                sprite->Play("Idle");
                break;
            case SDLK_d:
                transform->velocity.x = 0;
                sprite->Play("Idle");
                break;
            default:
                break;
            }
        }
    }
};