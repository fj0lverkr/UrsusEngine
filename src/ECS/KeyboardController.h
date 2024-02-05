#pragma once
#include "../Game.h"
#include "Components.h"
#include <set>

class KeyboardController : public Component
{
private:
    std::set<SDL_Keycode> pressedKeys;

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
                    sprite->Play("Idle");
                }
                else
                {
                    transform->velocity.y *= -1;
                }

                break;
            case SDLK_a:
                pressedKeys.erase(pressedKeys.find(Game::event.key.keysym.sym));
                if (pressedKeys.find(SDLK_d) == pressedKeys.end())
                {
                    transform->velocity.x = 0;
                    sprite->Play("Idle");
                }
                else
                {
                    transform->velocity.x *= -1;
                }
                break;
            case SDLK_s:
                pressedKeys.erase(pressedKeys.find(Game::event.key.keysym.sym));
                if (pressedKeys.find(SDLK_w) == pressedKeys.end())
                {
                    transform->velocity.y = 0;
                    sprite->Play("Idle");
                }
                else
                {
                    transform->velocity.y *= -1;
                }
                break;
            case SDLK_d:
                pressedKeys.erase(pressedKeys.find(Game::event.key.keysym.sym));
                if (pressedKeys.find(SDLK_a) == pressedKeys.end())
                {
                    transform->velocity.x = 0;
                    sprite->Play("Idle");
                }
                else
                {
                    transform->velocity.x *= -1;
                }
                break;
            default:
                break;
            }
        }
    }
};