#pragma once
#include "../Game.h"
#include "Components.h"

struct KeyboardController : public Component
{
    TransformComponent *transform;

    void init() override
    {
        transform = &entity->GetComponent<TransformComponent>();
    }

    void update() override
    {
        if (Game::event.type == SDL_KEYDOWN)
        {
            switch (Game::event.key.keysym.sym)
            {
            case SDLK_w:
                transform->velocity.y = -1;
                break;
            case SDLK_a:
                transform->velocity.x = -1;
                break;
            case SDLK_s:
                transform->velocity.y = 1;
                break;
            case SDLK_d:
                transform->velocity.x = 1;
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
                break;
            case SDLK_a:
                transform->velocity.x = 0;
                break;
            case SDLK_s:
                transform->velocity.y = 0;
                break;
            case SDLK_d:
                transform->velocity.x = 0;
                break;
            default:
                break;
            }
        }
    }
};