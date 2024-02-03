#pragma once
#include "Components.h"
#include "Vector2D.h"

struct TransformComponent : public Component
{
    Vector2D position;
    Vector2D velocity;

    int speed;

    TransformComponent()
    {
        position.x = position.y = 0.0f;
        speed = 3;
    }

    TransformComponent(float x, float y)
    {
        position.x = x;
        position.y = y;
        speed = 3;
    }

    TransformComponent(float x, float y, int speed)
    {
        this->speed = speed;
        TransformComponent(x, y);
    }

    void init() override
    {
        velocity.x = velocity.y = 0;
    }

    void update() override
    {
        position.x += velocity.x * speed;
        position.y += velocity.y * speed;
    }
};