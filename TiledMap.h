#pragma once

#include "Game.h"

class TiledMap
{
private:
    SDL_Rect srcRect, destRect;
    SDL_Texture *dirtTexture, *grassTexture, *waterTexture;
    int map[20][25];

public:
    TiledMap();
    ~TiledMap();

    void LoadMap(int arr[20][25]);
    void DrawMap();
};