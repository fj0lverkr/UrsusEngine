#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "Components.hpp"

class ColliderComponent : public Component
{
private:
    const int ANCHORSIZE = 5;
    boost::uuids::random_generator gen;
    Uint64 type;
    SDL_Texture* texture = {};

public:
    const enum ColliderType
    {
        Box,
        AnchorTop,
        AnchorCenter,
        AnchorBottom,
    };
    SDL_Rect collider = {};
    std::string tag;
    std::string uuid;

    TransformComponent* transform = {};

    ColliderComponent(std::string t)
    {
        tag = t;
        type = ColliderType::Box;
    }

    ColliderComponent(std::string t, Uint64 type)
    {
        tag = t;
        this->type = type;
    }

    void init() override
    {
        texture = TextureManager::LoadTexture("assets/placeholder.png");
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
        switch (this->type)
        {
        case ColliderType::AnchorBottom:
            collider.y = (transform->position.y + transform->height * transform->scale) - ANCHORSIZE - Game::camera.GetViewFinder().y;
            collider.x = transform->position.x + static_cast<float>(transform->width * transform->scale) / 2 - static_cast<float>(ANCHORSIZE) / 2 - Game::camera.GetViewFinder().x;
            collider.h = collider.w = ANCHORSIZE;
            break;
        case ColliderType::AnchorCenter:
            break;
        case ColliderType::AnchorTop:
            break;
        case ColliderType::Box:
        default:
            collider.x = static_cast<int>(transform->position.x) - Game::camera.GetViewFinder().x;
            collider.y = static_cast<int>(transform->position.y) - Game::camera.GetViewFinder().y;
            collider.w = transform->width * transform->scale;
            collider.h = transform->height * transform->scale;
            break;
        }
    }

    void draw() override
    {
        TextureManager::Draw(texture, {0, 0, 32, 32}, {collider.x, collider.y, collider.w, collider.h}, SDL_FLIP_NONE);
    }

    ~ColliderComponent()
    {
        SDL_DestroyTexture(texture);
    }

    Uint64 getType() const
    {
        return this->type;
    }
};