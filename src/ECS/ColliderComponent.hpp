#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "Components.hpp"

class ColliderComponent : public Component
{
private:
    boost::uuids::random_generator gen;

public:
    SDL_Rect collider = {};
    std::string tag;
    std::string uuid;

    TransformComponent *transform = new TransformComponent();

    ColliderComponent(std::string t)
    {
        tag = t;
    }

    void init() override
    {
        if (!entity->hasComponent<TransformComponent>())
        {
            entity->addComponent<TransformComponent>();
        }
        transform = &entity->GetComponent<TransformComponent>();

        boost::uuids::uuid id = gen();
        this->uuid = boost::uuids::to_string(id);

        if (!entity->hasGroup(Game::groupColliders))
        {
            entity->addGroup(Game::groupColliders);
        }
    }

    void update() override
    {
        collider.x = static_cast<int>(transform->position.x) - Game::camera.GetViewFinder().x;
        collider.y = static_cast<int>(transform->position.y) - Game::camera.GetViewFinder().y;
        collider.w = transform->width * transform->scale;
        collider.h = transform->height * transform->scale;
    }
};