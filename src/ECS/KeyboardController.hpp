#pragma once
#include "../Game.hpp"
#include "Components.hpp"
#include "EventSystem/KeyboardEventReceiver.hpp"

class KeyboardController : public Component
{
private: 
    TransformComponent* transform = {};
    SpriteComponent* sprite = {};
    KeyboardEventReceiver handler;

public:
    void init() override
    {
        transform = &entity->GetComponent<TransformComponent>();
        sprite = &entity->GetComponent<SpriteComponent>();
        handler.setTransform(transform);
        SubscribeToEvents(&handler);
    }

    void update() override
    {
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
};