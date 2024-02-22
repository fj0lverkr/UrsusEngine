#pragma once
#include "Components.hpp"
#include "Vector2D.hpp"
#include <SDL2/SDL.h>
#include <vector>

struct PolygonTransformComponent : public Component
{
	Vector2D position;
	Vector2D velocity;

    SDL_Point* points;
    int pointCount;
	int scale;
	int speed;

    PolygonTransformComponent(float x, float y, std::vector<SDL_Point> points, int scale, int speed)
    {
        position = { x, y };
        velocity = { 0, 0 };
        pointCount = points.size();
        this->points = new SDL_Point[pointCount];
        for (int i = 0; i < pointCount; i++) {
            this->points[i].x = x + points[i].x * scale;
            this->points[i].y = y + points[i].y * scale;
        }
        this->scale = scale;
        this->speed = speed;
    }

    void init() override
    {
        velocity.Zero();
    }

    void update() override
    {
        position.x += velocity.x * speed;
        position.y += velocity.y * speed;
    }
};