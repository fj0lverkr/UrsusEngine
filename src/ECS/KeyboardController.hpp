#pragma once
#include "../Game.hpp"
#include "Components.hpp"

class KeyboardController : public Component
{
private: 
    TransformComponent* transform = {};
    SpriteComponent* sprite = {};
    SDL_Event event = {};

    void resetVelocity(bool isHorizontal)
    {
        if (isHorizontal)
        {
            if (transform->velocity.x > 0)
            {
                transform->velocity.x = 1;
            }
            else if (transform->velocity.x < 0)
            {
                transform->velocity.x = -1;
            }
        }
        else
        {
            if (transform->velocity.y > 0)
            {
                transform->velocity.y = 1;
            }
            else if (transform->velocity.y < 0)
            {
                transform->velocity.y = -1;
            }
        }
    }

public:
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
                        resetVelocity(true);
                        break;
                    case SDLK_s:
                        if (transform->velocity.y > 0)
                        {
                            transform->velocity.y = 0;
                        }
                        resetVelocity(true);
                        break;
                    case SDLK_a:
                        if (transform->velocity.x < 0)
                        {
                            transform->velocity.x = 0;
                        }
                        resetVelocity(false);
                        break;
                    case SDLK_d:
                        if (transform->velocity.x > 0)
                        {
                            transform->velocity.x = 0;
                        }
                        resetVelocity(false);
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

            if (transform->velocity.IsZero())
            {
                sprite->Play("Idle");
            }
            else
            {
                // set the correct animation
                if (transform->velocity.x == 0)
                {
                    sprite->Play("WalkFront");
                    sprite->spriteFlip = transform->velocity.y > 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
                }
                else
                {
                    sprite->Play("WalkSide");
                    sprite->spriteFlip = transform->velocity.x > 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
                    if (transform->velocity.y != 0)
                    {
                        transform->velocity.Normalize();
                    }
                }
            }
        }
    }
};