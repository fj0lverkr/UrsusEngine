#pragma once

#include "Game.h"

class TiledMap
{
private:
    SDL_Rect srcRect, destRect;
    SDL_Texture *dirtTexture, *grassTexture, *waterTexture;

public:
    TiledMap();
    ~TiledMap();

    void Load();
    void Draw();
};