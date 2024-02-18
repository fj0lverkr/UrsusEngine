#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "Components.hpp"

class AnchorComponent : public Component
{
private:
    const int ANCHORSIZE = 5;
    boost::uuids::random_generator gen;
    Uint64 type;
    SDL_Texture* texture = {};

public:
    const enum AnchorType
    {
        AnchorTop,
        AnchorCenter,
        AnchorBottom,
    };
    SDL_Rect collider = {};
    std::string tag;
    std::string uuid;

    TransformComponent* transform = {};

    AnchorComponent(std::string t, Uint64 type)
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

        if (!entity->hasGroup(Game::groupColliders) && !entity->hasGroup(Game::groupPlayers) &&!entity->hasGroup(Game::groupEnemies))
        {
            entity->addGroup(Game::groupColliders);
        }
    }

    void update() override
    {
        switch (this->type)
        {
        case AnchorType::AnchorBottom:
            collider.y = (transform->position.y + transform->height * transform->scale) - ANCHORSIZE - Game::camera.GetViewFinder().y;
            collider.x = transform->position.x + static_cast<float>(transform->width * transform->scale) / 2 - static_cast<float>(ANCHORSIZE) / 2 - Game::camera.GetViewFinder().x;
            collider.h = collider.w = ANCHORSIZE;
            break;
        case AnchorType::AnchorCenter:
            break;
        case AnchorType::AnchorTop:
            break;
        default:
            break;
        }
    }

    void draw() override
    {
        TextureManager::Draw(texture, { 0, 0, 32, 32 }, { collider.x, collider.y, collider.w, collider.h }, SDL_FLIP_NONE);
    }

    ~AnchorComponent()
    {
        SDL_DestroyTexture(texture);
    }

    Uint64 getType() const
    {
        return this->type;
    }
};