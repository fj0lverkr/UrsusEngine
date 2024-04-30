#pragma once

#include "ECS.hpp"
#include "TileComponent.hpp"
#include "Game.hpp"
#include <SDL2/SDL.h>

using namespace std;

struct AnimatedTileFrame
{
    TileComponent frameTile;
    int duration;

    ~AnimatedTileFrame() {};

    AnimatedTileFrame(TileComponent frameTile, int duration)
    {
        this->frameTile = frameTile;
        this->duration = duration;
    }
};

class AnimatedTileComponent : public Component
{
private:
    int currentFrameId = 0;
    vector<AnimatedTileFrame> tileFrames;
    AnimatedTileFrame* currentFrame = nullptr;

public:
    ~AnimatedTileComponent() {}

    AnimatedTileComponent(vector<AnimatedTileFrame> frames)
    {
        tileFrames = frames;
        currentFrame = &frames[currentFrameId];
    }

    void update()
    {
        int animationSpeed = currentFrame->duration;
        int numFrames = static_cast<int>(tileFrames.size()) - 1;
        currentFrameId = static_cast<int>((SDL_GetTicks64() / animationSpeed) % numFrames);
        currentFrame = &tileFrames[currentFrameId];
    }

    void draw()
    {
        currentFrame->frameTile.draw();
    }
};