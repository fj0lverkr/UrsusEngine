#include "TiledMapManager.hpp"
#include "Game.hpp"
#include "TextureManager.hpp"
#include <map>
#include <string>
#include <vector>

TiledMapManager::TiledMapManager()
{
}

TiledMapManager::~TiledMapManager()
{
}

void TiledMapManager::loadMap(std::string filePath, int scaleFactor)
{
	scaleFactor = scaleFactor < 1 ? 1 : scaleFactor;
	tmx::Map map;
	std::map<uint32_t, const char*> tilesetTextureCollection;
	std::map<uint32_t, const std::vector<tmx::Tileset::Tile>> tilesetSpecialTilesCollection;

	if (map.load(filePath))
	{
		//get dimensions
		auto& mapDimensions = map.getTileCount();
		int cols = mapDimensions.x;
		int rows = mapDimensions.y;

		auto& tileSize = map.getTileSize();
		int tileWidth = tileSize.x;
		int tileHeight = tileSize.y;

		//get tilesets
		auto& tilesets = map.getTilesets();
		for (auto& tileset : tilesets)
		{
			const char* imagePath = tileset.getImagePath().c_str();
			const std::vector<tmx::Tileset::Tile> specialTiles = tileset.getTiles(); 
			tilesetTextureCollection.insert(std::pair<uint32_t, const char*>(tileset.getFirstGID(), imagePath));
			tilesetSpecialTilesCollection.insert(std::pair<uint32_t, const std::vector<tmx::Tileset::Tile>>(tileset.getFirstGID(), specialTiles));
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
			for (int y = 0; y < rows; y++)
			{
				for (int x = 0; x < cols; x++)
				{
					int tileIndex = x + (y * cols);
					auto currentGid = tiles[tileIndex].ID;

					if (currentGid == 0)
					{
						//skip empty tiles
						continue;
					}

					auto tilesetGid = -1;
					for (auto& ts : tilesetTextureCollection) {
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
					SDL_Texture *tex = TextureManager::LoadTexture(tilesetTextureCollection[tilesetGid]);
					SDL_QueryTexture(tex, NULL, NULL, &tileSheetWidth, &tileSheetHeight);

					// Calculate the area on the tilesheet to draw from.
					int srcX = (currentGid % (tileSheetWidth / tileWidth)) * tileWidth;
					int srcY = (currentGid / (tileSheetWidth / tileWidth)) * tileHeight;

					int posX = x * tileWidth * scaleFactor;
					int posY = y * tileHeight * scaleFactor;

					Game::AddTile(srcX, srcY, posX, posY, tilesetTextureCollection[tilesetGid],tileWidth, scaleFactor);
					// TODO: bring more meta-data into Game so we can do more with the tiles than just draw them.
				}
			}
		}
	}
	else
	{
		//throw error?
	}
}
