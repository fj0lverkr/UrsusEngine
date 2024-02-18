#pragma once
#include <SDL2/SDL.h>
#include <map>
#include <vector>
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include "TileCollider.hpp"

class TiledMapManager
{
private:
	void AddTile(int srcX, int srcY, int x, int y, const char* tilesetPath, int tileSize, int scaleFactor, std::vector<TileCollider> &colliders, bool debug) const;

public:
	TiledMapManager();
	~TiledMapManager();
	void loadMap(std::string filePath, int scaleFactor = 0, bool debug = false);
};