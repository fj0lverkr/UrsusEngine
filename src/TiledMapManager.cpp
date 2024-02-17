#include "TiledMapManager.hpp"
#include "Game.hpp"
#include "TextureManager.hpp"
#include "ECS/ECS.hpp"
#include "ECS/Components.hpp"

extern Manager manager;

TiledMapManager::TiledMapManager(size_t tileGroup, size_t colliderGroup)
{
	tilesGroup = tileGroup;
	collidersGroup = colliderGroup;
}

TiledMapManager::~TiledMapManager()
{
}

void TiledMapManager::loadMap(std::string filePath, int scaleFactor, bool debug)
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
			// Grab the associated image
			const char* imagePath = tileset.getImagePath().c_str();
			tilesetTextureCollection.insert(std::pair<uint32_t, const char*>(tileset.getFirstGID(), imagePath));

			// Grab the tiles that have non-empty objectgroups
			std::vector<tmx::Tileset::Tile> tsTiles;
			for (auto& t : tileset.getTiles())
			{
				if (!t.objectGroup.getObjects().empty())
				{
					tsTiles.emplace_back(t);
				}
			}
			tilesetSpecialTilesCollection.insert(std::pair<uint32_t, const std::vector<tmx::Tileset::Tile>>(tileset.getFirstGID(), tsTiles));
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

					// Get the objects from the current tile on the current tileset.
					std::vector<tmx::Tileset::Tile> specialTiles = tilesetSpecialTilesCollection[tilesetGid];
					tmx::ObjectGroup currentTileObjectGroup;
					std::vector<tmx::Object> currentTileObjects;
					for (auto& t : specialTiles)
					{
						if (t.ID == currentGid)
						{
							currentTileObjectGroup = t.objectGroup;
						}
					}
					currentTileObjects = currentTileObjectGroup.getObjects();

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



					// Process TileObjects
					std::vector<SDL_Rect> AABBColliders;
					for (auto& o : currentTileObjects)
					{
						// Object should be of class TileCollision in Tiled to be recognized as a tile collision object,
						// we can expand this to a switch statement when/if needed.
						if (o.getClass() == "TileCollision")
						{
							if (o.getShape() == tmx::Object::Shape::Ellipse || o.getShape() == tmx::Object::Shape::Rectangle)
							{
								//use getAABB() to get the bounding box for the object
								auto& aabb = o.getAABB();
								SDL_Rect r{};
								r.x = (int)aabb.left;
								r.y = (int)aabb.top;
								r.w = (int)aabb.width;
								r.h = (int)aabb.height;
								AABBColliders.push_back(r);
							}
							else
							{
								//use getPoints() the get a vector of points to draw a more complex shape
							}
						}
						
					}

					// Draw the Tile
					AddTile(srcX, srcY, posX, posY, tilesetTextureCollection[tilesetGid], tileWidth, scaleFactor, AABBColliders, debug);
				}
			}
		}
	}
	else
	{
		//throw error?
	}
}

void TiledMapManager::AddTile(int srcX, int srcY, int x, int y, const char* tilesetPath, int tileSize, int scaleFactor, const std::vector<SDL_Rect> &colliders, bool debug) const
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, x, y, tilesetPath, tileSize, scaleFactor);
	tile.addGroup(tilesGroup);
	for (SDL_Rect collider : colliders)
	{
		auto& box(manager.addEntity());
		int boxX = x + collider.x * scaleFactor;
		int boxY = y + collider.y * scaleFactor;
		box.addComponent<TransformComponent>(boxX, boxY, collider.w, collider.h, scaleFactor);
		if (debug)
		{
			box.addComponent<SpriteComponent>("assets/placeholder.png", false);
		}

		box.addComponent<ColliderComponent>("TileCollision");
		box.addGroup(collidersGroup);
	}
}
