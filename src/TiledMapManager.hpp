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
	void AddTile(int srcX, int srcY, float x, float y, std::string tilesetAssetId, int tileSize, int scaleFactor, std::vector<TileCollider> &colliders, bool debug) const;

public:
	TiledMapManager();
	~TiledMapManager();
	void LoadMap(std::string mapAssetId, int scaleFactor = 0, bool debug = false);
};