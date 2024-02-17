#pragma once
#include <string>
#include <map>
#include <vector>
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>

struct TiledMapManager
{
	TiledMapManager();
	~TiledMapManager();
	static void loadMap(std::string filePath, int scaleFactor = 0);
};