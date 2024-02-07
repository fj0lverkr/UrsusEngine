#pragma once
#include "Components.h"
#include "Vector2D.h"

struct TransformComponent : public Component
{
    Vector2D position;
    Vector2D velocity;

    int width;
    int height;
    int scale;
    int speed;

    TransformComponent()
    {
        position.Zero();
        width = height = 32;
        scale = 1;
        speed = 3;
    }

    TransformComponent(float x, float y)
    {
        width = height = 32;
        scale = 1;
        speed = 3;
        position.x = x;
        position.y = y;
    }

    TransformComponent(float x, float y, int w, int h, int s)
    {
        speed = 3;
        position.x = x;
        position.y = y;
        width = w;
        height = h;
        scale = s;
    }

    TransformComponent(float x, float y, int speed)
    {
        width = height = 32;
        scale = 1;
        position.x = x;
        position.y = y;
        this->speed = speed;
    }

    TransformComponent(float x, float y, int w, int h, int s, int speed)
    {
        position.x = x;
        position.y = y;
        width = w;
        height = h;
        scale = s;
        this->speed = speed;
    }

    void init() override
    {
        velocity.Zero();
    }

    void update() override
    {
        if (velocity.x != 0.0f && velocity.y != 0.0f)
        {
            // velocity.Normalize();
        }
        position.x += velocity.x * speed;
        position.y += velocity.y * speed;
    }
};