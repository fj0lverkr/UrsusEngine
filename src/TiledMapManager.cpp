#include "TiledMapManager.hpp"
#include "Game.hpp"
#include "TextureManager.hpp"
#include <map>
#include <string>

TiledMapManager::TiledMapManager()
{
}

TiledMapManager::~TiledMapManager()
{
}

void TiledMapManager::loadMap(std::string filePath, int scaleFactor)
{
	tmx::Map map;
	std::map<uint32_t, const char*> tilesetCollection;

	if (map.load(filePath))
	{
		//get dimensions
		auto& mapDimensions = map.getTileCount();
		int rows = mapDimensions.x;
		int cols = mapDimensions.y;

		auto& tileSize = map.getTileSize();
		int tileWidth = tileSize.x;
		int tileHeight = tileSize.y;

		//get tilesets
		auto& tilesets = map.getTilesets();
		for (auto& tileset : tilesets)
		{
			const char* imagePath = tileset.getImagePath().c_str();
			tilesetCollection.insert(std::pair<uint32_t, const char*>(tileset.getFirstGID(), imagePath));
		}

		//get layers
		auto& layers = map.getLayers();
		for (auto& layer : layers)
		{
			if (layer->getType() != tmx::Layer::Type::Tile)
			{
				//skip none-tile layers (for now)
				continue;
			}
			auto* tileLayer = dynamic_cast<const tmx::TileLayer*>(layer.get());
			auto& tiles = tileLayer->getTiles();
			for (int y = 0; y < cols; y++)
			{
				for (int x = 0; x < rows; x++)
				{
					int tileIndex = x + (y * cols);
					auto currentGid = tiles[tileIndex].ID;

					if (currentGid == 0)
					{
						//skip empty tiles
						continue;
					}

					auto tilesetGid = -1;
					for (auto& ts : tilesetCollection) {
						if (ts.first <= currentGid) {
							tilesetGid = ts.first;
							break;
						}
					}

					if (tilesetGid == -1) {
						//skip if no tileset was found for the tile
						continue;
					}

					// Normalize the GID.
					currentGid -= tilesetGid;

					// Find the dimensions of the tile sheet.
					int tileSheetWidth = 0;
					int tileSheetHeight = 0;
					SDL_Texture *tex = TextureManager::LoadTexture(tilesetCollection[tilesetGid]);
					SDL_QueryTexture(tex, NULL, NULL, &tileSheetWidth, &tileSheetHeight);

					// Calculate the area on the tilesheet to draw from.
					int srcX = (currentGid % (tileSheetWidth / tileWidth)) * tileWidth;
					int srcY = (currentGid / (tileSheetWidth / tileWidth)) * tileHeight;

					auto destX = x * tileWidth;
					auto destY = y * tileHeight;

					Game::AddTile(srcX, srcY, destX, destY, tilesetCollection[tilesetGid], scaleFactor);
				}
			}
		}
	}
	else
	{
		//throw error?
	}
}
