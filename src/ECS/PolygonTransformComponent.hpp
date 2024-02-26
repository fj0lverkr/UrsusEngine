#pragma once
#include "Components.hpp"
#include "Vector2D.hpp"
#include <SDL2/SDL.h>
#include <vector>

struct PolygonTransformComponent : public Component
{
	Vector2D position;
	Vector2D velocity;

    SDL_FPoint* points;
    size_t pointCount;
	int scale;
	int speed;

    PolygonTransformComponent()
    {

        this->scale = 0;
        this->speed = 0;
        position = { 0.0f, 0.0f };
        velocity = { 0.0f, 0.0f };
        pointCount = 1;
        this->points = new SDL_FPoint[pointCount];
        for (int i = 0; i < pointCount - 1; i++) {
            this->points[i].x = 0.0f + points[i].x * scale;
            this->points[i].y = 0.0f + points[i].y * scale;
        }
    }

    PolygonTransformComponent(float x, float y, std::vector<SDL_FPoint> points, int scale, int speed)
    {
        position = { x, y };
        velocity = { 0, 0 };
        pointCount = points.size() + 1;
        this->points = new SDL_FPoint[pointCount];
        for (int i = 0; i < pointCount -1; i++) {
            this->points[i].x = x + points[i].x * scale;
            this->points[i].y = y + points[i].y * scale;
        }

        this->scale = scale;
        this->speed = speed;
    }

    void init() override
    {
        //add one extra point so the polygon is a closed shape
        this->points[pointCount - 1].x = this->points[0].x;
        this->points[pointCount - 1].y = this->points[0].y;
        velocity.Zero();
    }

    void update() override
    {
        position.x += velocity.x * speed;
        position.y += velocity.y * speed;
    }
};