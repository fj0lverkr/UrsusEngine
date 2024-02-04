#include "TiledMap.h"
#include "Game.h"
#include <fstream>

TiledMap::TiledMap()
{
}

TiledMap::~TiledMap()
{
}

void TiledMap::LoadMap(std::string path, int sizeX, int sizeY)
{
    char tile;
    std::fstream mapfile;
    mapfile.open(path);

    for (int y = 0; y < sizeY; y++)
    {
        for (int x = 0; x < sizeX; x++)
        {
            mapfile.get(tile);
            Game::AddTile(atoi(&tile), x * 32, y * 32);
            mapfile.ignore(); // skip comma's
        }
    }

    mapfile.close();
}
