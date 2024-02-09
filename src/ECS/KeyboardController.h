#pragma once
#include "../Game.h"
#include "Components.h"

struct KeyboardController : public Component
{
    TransformComponent *transform;
    SpriteComponent *sprite;
    SDL_Event event;

    void init() override
    {
        transform = &entity->GetComponent<TransformComponent>();
        sprite = &entity->GetComponent<SpriteComponent>();
    }

    void update() override
    {
        while (SDL_PollEvent(&event))
        {
            if (event.key.repeat == 0)
            {
                if (event.type == SDL_KEYDOWN)
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_w:
                        transform->velocity.y = -1;
                        sprite->Play("WalkSide");
                        break;
                    case SDLK_a:
                        transform->velocity.x = -1;
                        sprite->Play("WalkSide");
                        sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                        break;
                    case SDLK_s:
                        transform->velocity.y = 1;
                        sprite->Play("WalkSide");
                        break;
                    case SDLK_d:
                        transform->velocity.x = 1;
                        sprite->Play("WalkSide");
                        sprite->spriteFlip = SDL_FLIP_NONE;
                        break;
                    default:
                        break;
                    }
                }

                if (event.type == SDL_KEYUP)
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_w:
                        if (transform->velocity.y < 0)
                        {
                            transform->velocity.y = 0;
                        }
                        break;
                    case SDLK_a:
                        if (transform->velocity.x < 0)
                        {
                            transform->velocity.x = 0;
                        }
                        break;
                    case SDLK_s:
                        if (transform->velocity.y > 0)
                        {
                            transform->velocity.y = 0;
                        }
                        break;
                    case SDLK_d:
                        if (transform->velocity.x > 0)
                        {
                            transform->velocity.x = 0;
                        }
                        break;
                    case SDLK_ESCAPE:
                        Game::isRunning = false;
                        break;
                    default:
                        break;
                    }
                }
            }

            if (event.type == SDL_QUIT)
            {
                Game::isRunning = false;
            }

            // Reset sprite
            if (transform->velocity.IsZero())
            {
                sprite->Play("Idle");
            }
            else
            {
                // Scrolling tiles should be used without normalization (for now at least).
                // transform->velocity.Normalize();
            }
        }
    }
};