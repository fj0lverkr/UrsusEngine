#include "TiledMapManager.hpp"
#include "Game.hpp"
#include "ECS/Components.hpp"

extern Manager manager;

TiledMapManager::TiledMapManager()
{
}

TiledMapManager::~TiledMapManager()
{
}

void TiledMapManager::loadMap(std::string mapAssetId, int scaleFactor, bool debug)
{
	scaleFactor = scaleFactor < 1 ? 1 : scaleFactor;
	tmx::Map map;
	std::map<uint32_t, std::string> tilesetTextureCollection;
	std::map<uint32_t, const std::vector<tmx::Tileset::Tile>> tilesetSpecialTilesCollection;

	if (Game::assets->LoadtiledMap(mapAssetId, &map))
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
			// Grab the associated image and put it in our AssetManager
			Game::assets->AddTexture(tileset.getName(), tileset.getImagePath().c_str());
			tilesetTextureCollection.insert(std::pair<uint32_t, std::string>(tileset.getFirstGID(), tileset.getName()));

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
					SDL_Texture *tex = Game::assets->GetTexture(tilesetTextureCollection[tilesetGid]);
					SDL_QueryTexture(tex, NULL, NULL, &tileSheetWidth, &tileSheetHeight);

					// Calculate the area on the tilesheet to draw from.
					int srcX = (currentGid % (tileSheetWidth / tileWidth)) * tileWidth;
					int srcY = (currentGid / (tileSheetWidth / tileWidth)) * tileHeight;

					float posX = static_cast<float>(x * tileWidth * scaleFactor);
					float posY = static_cast<float>(y * tileHeight * scaleFactor);



					// Process TileObjects
					std::vector<TileCollider> tileColliders;
					for (auto& o : currentTileObjects)
					{
						// Object should be of class TileCollision in Tiled to be recognized as a tile collision object,
						// we can expand this to a switch statement when/if needed.
						if (o.getClass() == "TileCollision")
						{
							auto& tag = o.getName() == "" ? o.getClass() : o.getName();
							if (o.getShape() == tmx::Object::Shape::Ellipse || o.getShape() == tmx::Object::Shape::Rectangle)
							{
								//use getAABB() to get the bounding box for the object
								auto& aabb = o.getAABB();
								SDL_FRect r{};
								r.x = aabb.left;
								r.y = aabb.top;
								r.w = aabb.width;
								r.h = aabb.height;
								tileColliders.emplace_back(TileCollider {r, tag});
							}
							else
							{
								std::vector<SDL_FPoint> tileColliderPoints = {};
								//use getPoints() the get a vector of points to draw a more complex shape
								//this means we need to add a different type of ColliderComponent as the regular one takes an SDL_Rect for the shape.
								for (auto& p : o.getPoints())
								{
									SDL_FPoint point = { p.x, p.y };
									tileColliderPoints.emplace_back(point);
								}
								tileColliders.emplace_back(TileCollider{ o.getPosition().x, o.getPosition().y, tileColliderPoints, tag });
							}
						}
						
					}

					// Draw the Tile
					AddTile(srcX, srcY, posX, posY, tilesetTextureCollection[tilesetGid], tileWidth, scaleFactor, tileColliders, debug);
				}
			}
		}
	}
	else
	{
		//throw error?
	}
}

void TiledMapManager::AddTile(int srcX, int srcY, float x, float y, std::string tilesetAssetId, int tileSize, int scaleFactor, std::vector<TileCollider> &colliders, bool debug) const
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, x, y, tilesetAssetId, tileSize, scaleFactor);
	tile.addGroup(Game::groupMap);
	for (auto& c : colliders)
	{
		auto& box(manager.addEntity());
		if (c.getColliderType() == Rectangle)
		{
			float boxX = x + c.getColliderRect().x * scaleFactor;
			float boxY = y + c.getColliderRect().y * scaleFactor;
			box.addComponent<TransformComponent>(boxX, boxY, c.getColliderRect().w, c.getColliderRect().h, scaleFactor);
			box.addComponent<ColliderComponent>(c.getColliderTag(), ColliderType::AABB);

			if (debug)
			{
				box.addComponent<SpriteComponent>("Placeholder", false);
			}
		}
		else
		{
			float posX = x + c.getColliderPosition().x * scaleFactor;
			float posY = y + c.getColliderPosition().y * scaleFactor;
			box.addComponent<PolygonTransformComponent>(posX, posY, c.getColliderPoints(), scaleFactor, 0);
			box.addComponent<ColliderComponent>(c.getColliderTag(), ColliderType::Polygon);

			if (debug)
			{
				box.addComponent<PolygonLinesComponent>();
			}
		}
	}
}
