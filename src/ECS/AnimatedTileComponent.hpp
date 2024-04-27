#pragma once

#include "ECS.hpp"
#include "TileComponent.hpp"
#include "Game.hpp"
#include <SDL2/SDL.h>

using namespace std;

struct AnimatedTileFrame
{
    string assetId;
    int duration;

    ~AnimatedTileFrame() {};

    AnimatedTileFrame(string assetId, int duration)
    {
        this->assetId = assetId;
        this->duration = duration;
    }
};

class AnimatedTileComponent : public TileComponent
{
private:
    int currentFrameId = 0;
    vector<AnimatedTileFrame> tileFrames;
public:
    ~AnimatedTileComponent() {}

    AnimatedTileComponent(int srcX, int srcY, float posX, float posY, int tileSize, int scaleFactor, vector<AnimatedTileFrame> frames)
    {
        tileFrames = frames;
        TileComponent(srcX, srcY, posX, posY, tileFrames[currentFrameId].assetId, tileSize, scaleFactor);
    }

    void update()
    {
        TileComponent::update();
    }

    void swapFrame(int frameId)
    {
        texture = Game::assets->GetTexture(tileFrames[frameId].assetId);
    }

    void draw()
    {
        TileComponent::draw();
    }
};