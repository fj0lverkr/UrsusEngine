#include "TiledMap.h"
#include "Game.h"
#include <fstream>

TiledMap::TiledMap()
{
}

TiledMap::~TiledMap()
{
}

void TiledMap::LoadMap(std::string path, int sizeX, int sizeY, int scaleFactor)
{
    int scale = scaleFactor > 1 ? scaleFactor : 1;
    char c;
    std::fstream mapfile;
    mapfile.open(path);

    int srcX, srcY;

    for (int y = 0; y < sizeY; y++)
    {
        for (int x = 0; x < sizeX; x++)
        {
            mapfile.get(c);
            srcY = atoi(&c) * 32;
            mapfile.get(c);
            srcX = atoi(&c) * 32;
            Game::AddTile(srcX, srcY, x * 32 * scaleFactor, y * 32 * scale);
            mapfile.ignore(); // skip comma's
        }
    }

    mapfile.close();
}
