#pragma once

#include "ECS.hpp"
#include "TileComponent.hpp"
#include "Game.hpp"
#include <SDL2/SDL.h>

class AnimatedTileComponent : public TileComponent
{
public:
    ~AnimatedTileComponent() {}

    AnimatedTileComponent(int srcX, int srcY, float posX, float posY, std::string assetId, int tileSize, int scaleFactor)
    {
        TileComponent(srcX, srcY, posX, posY, assetId, tileSize, scaleFactor);
    }

    void update()
    {
        TileComponent::update();
    }

    void draw()
    {
        TileComponent::draw();
    }
};