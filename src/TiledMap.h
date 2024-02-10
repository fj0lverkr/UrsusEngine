#pragma once
#include <string>

struct TiledMap
{
    TiledMap();
    ~TiledMap();

    static void LoadMap(std::string path, int sizeX, int sizeY, int tileSize, int scaleFactor);
};