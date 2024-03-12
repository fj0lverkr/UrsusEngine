#pragma once
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "Components.hpp"

enum ColliderType
{
    AABB,
    Polygon,
};

class ColliderComponent : public Component
{
private:
    boost::uuids::random_generator gen;
    ColliderType type;

public:
    SDL_FRect collider = {};
    std::vector<SDL_FPoint> colliderPoints{ {0.0f, 0.0f} };
    std::string tag;
    std::string uuid;

    TransformComponent* transform = nullptr;
    PolygonTransformComponent* polyTransform = nullptr;

    ColliderComponent(std::string t, ColliderType type)
    {
        tag = t;
        this->type = type;
    }

    void init() override
    {
        switch (this->type)
        {
        case AABB:
            //setup transform, leave polyTransform nullptr.
            if (!entity->hasComponent<TransformComponent>())
            {
                entity->addComponent<TransformComponent>();
            }
            transform = &entity->GetComponent<TransformComponent>();
            break;
        case Polygon:
            //setup polytransform, leave transform nullptr.
            if (!entity->hasComponent<PolygonTransformComponent>())
            {
                entity->addComponent<PolygonTransformComponent>();
            }
            polyTransform = &entity->GetComponent<PolygonTransformComponent>();
            colliderPoints = std::vector<SDL_FPoint>(polyTransform->pointCount);
            break;
        default:
            break;

        }

        boost::uuids::uuid id = gen();
        this->uuid = boost::uuids::to_string(id);

        if (!entity->hasGroup(Game::groupColliders) && !entity->hasGroup(Game::groupPlayers) && !entity->hasGroup(Game::groupEnemies))
        {
            entity->AddGroup(Game::groupColliders);
        }
    }

    void update() override
    {
        switch (this->type)
        {
        case AABB:
            collider.x = transform->position.x - Game::camera.GetViewFinder().x;
            collider.y = transform->position.y - Game::camera.GetViewFinder().y;
            collider.w = transform->width * transform->scale;
            collider.h = transform->height * transform->scale;
            break;
        case Polygon:
            for (int i = 0; i < polyTransform->pointCount; i++)
            {
                colliderPoints[i].x = polyTransform->points[i].x - Game::camera.GetViewFinder().x;
                colliderPoints[i].y = polyTransform->points[i].y - Game::camera.GetViewFinder().y;
            }
            break;
        default:
            break;
        }
    }

    ColliderType getType() const
    {
        return this->type;
    }
};