#pragma once
#include "../Game.h"
#include "Components.h"
#include <set>

class KeyboardController : public Component
{
private:
    std::set<SDL_Keycode> pressedKeys;

    bool isDiagonal(SDL_Keycode refKeycode)
    {
        switch (refKeycode)
        {
        case SDLK_w:
            return pressedKeys.find(SDLK_a) != pressedKeys.end() || pressedKeys.find(SDLK_d) != pressedKeys.end();
            break;
        case SDLK_a:
            return pressedKeys.find(SDLK_w) != pressedKeys.end() || pressedKeys.find(SDLK_s) != pressedKeys.end();
            break;
        case SDLK_s:
            return pressedKeys.find(SDLK_a) != pressedKeys.end() || pressedKeys.find(SDLK_d) != pressedKeys.end();
            break;
        case SDLK_d:
            return pressedKeys.find(SDLK_w) != pressedKeys.end() || pressedKeys.find(SDLK_s) != pressedKeys.end();
            break;
        default:
            break;
        }
    }

    bool isMovementKey(SDL_Keycode refKeycode)
    {
        return refKeycode == SDLK_w || refKeycode == SDLK_a || refKeycode == SDLK_s || refKeycode == SDLK_d;
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
            switch (Game::event.key.keysym.sym)
            {
            case SDLK_w:
                pressedKeys.emplace(Game::event.key.keysym.sym); // elements in a set are always unique, so we can just add it.
                transform->velocity.y = -1;
                sprite->Play("WalkSide");
                break;
            case SDLK_a:
                pressedKeys.emplace(Game::event.key.keysym.sym);
                transform->velocity.x = -1;
                sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                sprite->Play("WalkSide");
                break;
            case SDLK_s:
                pressedKeys.emplace(Game::event.key.keysym.sym);
                transform->velocity.y = 1;
                sprite->Play("WalkSide");
                break;
            case SDLK_d:
                pressedKeys.emplace(Game::event.key.keysym.sym);
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
                pressedKeys.erase(pressedKeys.find(Game::event.key.keysym.sym));
                if (pressedKeys.find(SDLK_s) == pressedKeys.end())
                {
                    transform->velocity.y = 0;
                    if (!isDiagonal(Game::event.key.keysym.sym))
                    {
                        sprite->Play("Idle");
                    }
                }
                else
                {
                    if (transform->velocity.y < 0)
                    {
                        transform->velocity.y *= -1;
                    }
                }
                break;
            case SDLK_a:
                pressedKeys.erase(pressedKeys.find(Game::event.key.keysym.sym));
                if (pressedKeys.find(SDLK_d) == pressedKeys.end())
                {
                    transform->velocity.x = 0;
                    if (!isDiagonal(Game::event.key.keysym.sym))
                    {
                        sprite->Play("Idle");
                    }
                }
                else
                {
                    if (transform->velocity.x < 0)
                    {
                        transform->velocity.x *= -1;
                    }
                }
                break;
            case SDLK_s:
                pressedKeys.erase(pressedKeys.find(Game::event.key.keysym.sym));
                if (pressedKeys.find(SDLK_w) == pressedKeys.end())
                {
                    transform->velocity.y = 0;
                    if (!isDiagonal(Game::event.key.keysym.sym))
                    {
                        sprite->Play("Idle");
                    }
                }
                else
                {
                    if (transform->velocity.y > 0)
                    {
                        transform->velocity.y *= -1;
                    }
                }
                break;
            case SDLK_d:
                pressedKeys.erase(pressedKeys.find(Game::event.key.keysym.sym));
                if (pressedKeys.find(SDLK_a) == pressedKeys.end())
                {
                    transform->velocity.x = 0;
                    if (!isDiagonal(Game::event.key.keysym.sym))
                    {
                        sprite->Play("Idle");
                    }
                }
                else
                {
                    if (transform->velocity.x > 0)
                    {
                        transform->velocity.x *= -1;
                    }
                }
                break;
            default:
                break;
            }
        }
    }
};