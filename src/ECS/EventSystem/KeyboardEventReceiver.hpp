#pragma once
#include <SDL2/SDL.h>
#include "../Components.hpp"

class KeyboardEventReceiver : public EventReceiver
{
private:
    TransformComponent* transform = {};

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
    void setTransform(TransformComponent* t)
    {
        transform = t;
    }

    bool handleEvent(const SDL_Event* e) override
    {
        if (e->type == SDL_KEYDOWN || e->type == SDL_KEYUP)
        {
            if (e->key.repeat == 0)
            {
                if (e->type == SDL_KEYDOWN)
                {
                    switch (e->key.keysym.sym)
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

                if (e->type == SDL_KEYUP)
                {
                    switch (e->key.keysym.sym)
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
            return true;
        }
        else
        {
            return false;
        }
    }
};