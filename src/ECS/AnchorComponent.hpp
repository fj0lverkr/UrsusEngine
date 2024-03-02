#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "Game.hpp"
#include "Components.hpp"

class AnchorComponent : public Component
{
private:
    const float ANCHORSIZE = 5.0f;
    boost::uuids::random_generator gen;
    Uint64 type;
    SDL_Texture* texture = {};
    float customX = 0.0f;
    float customY = 0.0f;
    float offset;
    bool debug;

public:
    const enum AnchorType
    {
        AnchorTop,
        AnchorCenter,
        AnchorBottom,
        AnchorCustom,
    };

    SDL_FRect collider = {};
    std::string tag;
    std::string uuid;

    TransformComponent* transform = {};

    AnchorComponent(std::string t, float x, float y, bool debug = false)
    {
        tag = t;
        this->type = AnchorCustom;
        customX = x;
        customY = y;
        this->offset = 0.0f;
        this->debug = debug;
    }

    AnchorComponent(std::string t, Uint64 type, bool debug = false)
    {
        tag = t;
        this->type = type;
        this->offset = 0.0f;
        this->debug = debug;
    }

    AnchorComponent(std::string t, Uint64 type, float offsetTop, float offsetBottom, bool debug = false)
    {
        tag = t;
        this->type = type;
        this->offset = offsetTop - offsetBottom;
        this->debug = debug;
    }

    void init() override
    {
        texture = Game::assets->GetTexture("Placeholder");
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

        collider.h = collider.w = ANCHORSIZE;
    }

    void update() override
    {
        switch (this->type)
        {
        case AnchorType::AnchorBottom:
            collider.y = (transform->position.y + transform->height * transform->scale) - ANCHORSIZE - Game::camera.GetViewFinder().y + offset;
            collider.x = transform->position.x + static_cast<float>(transform->width * transform->scale) / 2 - ANCHORSIZE / 2 - Game::camera.GetViewFinder().x;
            break;
        case AnchorType::AnchorCenter:
            collider.y = transform->position.y + static_cast<float>(transform->height * transform->scale) / 2 - ANCHORSIZE / 2 - Game::camera.GetViewFinder().y + offset;
            collider.x = transform->position.x + static_cast<float>(transform->width * transform->scale) / 2 - ANCHORSIZE / 2 - Game::camera.GetViewFinder().x;
            break;
        case AnchorType::AnchorTop:
            collider.y = transform->position.y - Game::camera.GetViewFinder().y + offset;
            collider.x = transform->position.x + static_cast<float>(transform->width * transform->scale) / 2 - ANCHORSIZE / 2 - Game::camera.GetViewFinder().x;
            break;
        case AnchorType::AnchorCustom:
            // This places the center of the anchor at the custom position, relative to the scale of the entity transform.
            collider.y = (transform->position.y + customY * transform->scale) - ANCHORSIZE / 2 - Game::camera.GetViewFinder().y;
            collider.x = (transform->position.x + customX * transform->scale) - ANCHORSIZE / 2 - Game::camera.GetViewFinder().x;
            break;
        default:
            break;
        }
    }

    void draw() override
    {
        if (debug)
        {
            TextureManager::Draw(texture, { 0, 0, 32, 32 }, { collider.x, collider.y, collider.w, collider.h }, SDL_FLIP_NONE);
        }
    }

    ~AnchorComponent()
    {
    }

    Uint64 getType() const
    {
        return this->type;
    }
};