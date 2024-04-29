#include "TiledMapManager.hpp"
#include "Game.hpp"
#include "ECS/Components.hpp"

extern Manager manager;

using namespace std;

TiledMapManager::TiledMapManager()
{
}

TiledMapManager::~TiledMapManager()
{
}

void TiledMapManager::LoadMap(string mapAssetId, int scaleFactor, bool debug)
{
	scaleFactor = scaleFactor < 1 ? 1 : scaleFactor;
	tmx::Map tiledMap;
	map<uint32_t, string> tilesetTextureCollection;
	map<uint32_t, string> tilesetTileTextureCollection;
	map<uint32_t, const vector<tmx::Tileset::Tile>> tilesetSpecialTilesCollection;
	map<uint32_t, const vector<tmx::Tileset::Tile>> tilesetAnimatedTilesCollection;

	if (Game::assets->LoadtiledMap(mapAssetId, &tiledMap))
	{
		//get dimensions
		const tmx::Vector2u& mapDimensions = tiledMap.getTileCount();
		int cols = mapDimensions.x;
		int rows = mapDimensions.y;

		const tmx::Vector2u& tileSize = tiledMap.getTileSize();
		int tileWidth = tileSize.x;
		int tileHeight = tileSize.y;

		//get tilesets
		const vector<tmx::Tileset>& tilesets = tiledMap.getTilesets();
		for (const tmx::Tileset& tileset : tilesets)
		{
			// Grab the associated image and put it in our AssetManager
			if(tileset.getImagePath() != "")
			{
				// If the tileset has one composite image to pull from:
				Game::assets->AddTexture(tileset.getName(), tileset.getImagePath().c_str());
				tilesetTextureCollection.insert(pair<uint32_t, string>(tileset.getFirstGID(), tileset.getName()));
			}
			else
			{
				// if the tileset has a different image for each tile:
				for (const tmx::Tileset::Tile& tile : tileset.getTiles())
				{
					uint32_t tilesetTileId = tileset.getFirstGID() + tile.ID;
					string assetName = tileset.getName() + "_" + to_string(tile.ID);
					Game::assets->AddTexture(assetName, tile.imagePath.c_str());
					tilesetTileTextureCollection.insert(pair<uint32_t, string>(tilesetTileId, assetName));
				}
			}

			// Grab the tiles that have non-empty objectgroups and/or have frames
			vector<tmx::Tileset::Tile> tsTilesWithObjects;
			vector<tmx::Tileset::Tile> tsTilesWithFrames;
			for (const tmx::Tileset::Tile& t : tileset.getTiles())
			{
				if (!t.objectGroup.getObjects().empty())
				{
					tsTilesWithObjects.emplace_back(t);
				}
				if (!t.animation.frames.empty())
				{
					tsTilesWithFrames.emplace_back(t);
				}
			}
			tilesetSpecialTilesCollection.insert(pair<uint32_t, const vector<tmx::Tileset::Tile>>(tileset.getFirstGID(), tsTilesWithObjects));
			tilesetAnimatedTilesCollection.insert(pair<uint32_t, const vector<tmx::Tileset::Tile>>(tileset.getFirstGID(), tsTilesWithFrames));

			// cleanup these vectors
			vector<tmx::Tileset::Tile>().swap(tsTilesWithObjects);
			vector<tmx::Tileset::Tile>().swap(tsTilesWithFrames);
		}

		//get layers
		const vector<tmx::Layer::Ptr>& layers = tiledMap.getLayers();
		for (const tmx::Layer::Ptr& layer : layers)
		{
			if (layer->getType() != tmx::Layer::Type::Tile)
			{
				// MAP OBJECTS

				if (layer->getType() == tmx::Layer::Type::Object)
				{
					auto* objectGroup = dynamic_cast<const tmx::ObjectGroup*>(layer.get());
					auto& objects = objectGroup->getObjects();
					for (auto& o : objects)
					{
						bool shouldYSort = false;
						float x = o.getPosition().x * scaleFactor;
						float y = o.getPosition().y * scaleFactor;
						int w = 0;
						int h = 0;
						string assetId;
						SDL_Texture* tex;
						vector<tmx::Object> objectObjects;

						uint32_t currentGid = o.getTileID();

						if (currentGid == 0)
						{
							//skip empty objects
							continue;
						}

						auto tilesetGid = -1;
						for (pair<const uint32_t, string>& ts : tilesetTextureCollection) {
							if (ts.first <= currentGid) {
								tilesetGid = ts.first;
								break;
							}
						}

						if (tilesetGid == -1) {
							//skip if no tileset was found for the object
							continue;
						}

						if (tilesetTileTextureCollection.contains(o.getTileID()))
						{
							assetId = tilesetTileTextureCollection[o.getTileID()];
							tilesetGid = o.getTileID()/100 * 100 + 1;

						}
						else {
							assetId = tilesetTextureCollection[tilesetGid];
						}

						// Normalize the GID.
						currentGid -= tilesetGid;

						// Get the Objects within the Object (i.e. the colliders)
						vector<tmx::Tileset::Tile> specialTiles = tilesetSpecialTilesCollection[tilesetGid];
						tmx::ObjectGroup currentTileObjectGroup;
						vector<tmx::Property> objectProps;
						
						for (tmx::Tileset::Tile& t : specialTiles)
						{
							if (t.ID == currentGid)
							{
								currentTileObjectGroup = t.objectGroup;
								objectProps = t.properties;
							}
						}
						objectObjects = currentTileObjectGroup.getObjects();
						
						for (tmx::Property& p : objectProps)
						{
							if (p.getName() == "Ysorting")
							{
								shouldYSort = p.getBoolValue();
							}
						}
						
						// Set the texture
						tex = Game::assets->GetTexture(assetId);
						SDL_QueryTexture(tex, NULL, NULL, &w, &h);
						
						// Correct the y value
						y -= h * scaleFactor;

						// Process objectObjects
						vector<TileCollider> objectColliders = GetColliders(objectObjects);

						// Draw the Object
						AddObject(x, y, assetId, static_cast<float>(w), static_cast<float>(h), scaleFactor, objectColliders, shouldYSort, debug);

					}
				}
				continue;
			}

			// REGULAR TILES

			auto* tileLayer = dynamic_cast<const tmx::TileLayer*>(layer.get());
			const vector< tmx::TileLayer::Tile>& tiles = tileLayer->getTiles();
			for (int y = 0; y < rows; y++)
			{
				for (int x = 0; x < cols; x++)
				{
					int tileIndex = x + (y * cols);
					uint32_t currentGid = tiles[tileIndex].ID;

					if (currentGid == 0)
					{
						//skip empty tiles
						continue;
					}

					int tilesetGid = -1;
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

					auto isSameTile = [currentGid](tmx::Tileset::Tile t) { return t.ID == currentGid; };

					// look for the tile in our collection of animated tiles
					for (pair<const uint32_t, const vector<tmx::Tileset::Tile>>& animatedTiles : tilesetAnimatedTilesCollection)
					{
						if (animatedTiles.second.empty())
						{
							// skip for layers with no animated tiles.
							continue;
						}
						
						auto findTile = find_if(animatedTiles.second.begin(), animatedTiles.second.end(), isSameTile);

						if (findTile != animatedTiles.second.end())
						{
							// the current tile is animated
							vector<AnimatedTileFrame> tileFrames;
							tmx::Tileset::Tile tsAnimatedTile = *findTile;
							// we now have the tile as tsAnimatedTile, we can use its position and such to draw the animation later, now we need the frames
							for (tmx::Tileset::Tile::Animation::Frame f : tsAnimatedTile.animation.frames)
							{
								uint32_t currentAGid = f.tileID;

								if (currentAGid == 0)
								{
									//skip empty tiles
									continue;
								}

								int tilesetGid = -1;
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
								currentAGid -= tilesetGid;

								// Find the dimensions of the tile sheet.
								int tileSheetWidth = 0;
								int tileSheetHeight = 0;
								SDL_Texture* tex = Game::assets->GetTexture(tilesetTextureCollection[tilesetGid]);
								SDL_QueryTexture(tex, NULL, NULL, &tileSheetWidth, &tileSheetHeight);

								// Calculate the area on the tilesheet to draw from.
								int srcX = (currentGid % (tileSheetWidth / tileWidth)) * tileWidth;
								int srcY = (currentGid / (tileSheetWidth / tileWidth)) * tileHeight;

								float posX = static_cast<float>(x *tileWidth * scaleFactor);
								float posY = static_cast<float>(y * tileHeight * scaleFactor);

								// Create tile and frame
								TileComponent frameTile = { srcX, srcY, posX, posY, tilesetTextureCollection[tilesetGid], tileWidth, scaleFactor };
								AnimatedTileFrame frame = { &frameTile, static_cast<int>(f.duration) };
								tileFrames.emplace_back(frame);
							}

							/* TODO move the following 3 lines to a separate function and handle colliders,
							also the code above might be simplified as it now copies a lot from the regular tiles. */ 
							Entity& animatedTile = manager.addEntity();
							animatedTile.addComponent<AnimatedTileComponent>(tileFrames);
							animatedTile.AddGroup(Game::groupMapTiles);
							break;
						}
					}

					// Get the objects from the current tile on the current tileset.
					vector<tmx::Tileset::Tile> specialTiles = tilesetSpecialTilesCollection[tilesetGid];
					tmx::ObjectGroup currentTileObjectGroup;
					vector<tmx::Object> currentTileObjects;
					for (tmx::Tileset::Tile& t : specialTiles)
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
					vector<TileCollider> tileColliders = GetColliders(currentTileObjects);

					// Draw the Tile
					AddTile(srcX, srcY, posX, posY, tilesetTextureCollection[tilesetGid], tileWidth, scaleFactor, tileColliders, debug);
				}
			}
		}
	}
	else
	{
		// TODO decide what to do here: throw error?
	}
}

vector<TileCollider> TiledMapManager::GetColliders(vector<tmx::Object>& tileObjects)
{
	vector<TileCollider> tileColliders;
	for (tmx::Object& o : tileObjects)
	{
		// Object should be of class TileCollision in Tiled to be recognized as a tile collision object,
		// we can expand this to a switch statement when/if needed.
		if (o.getClass() == "TileCollision" || o.getClass() == "ObjectCollision")
		{
			const string& tag = o.getName() == "" ? o.getClass() : o.getName();
			if (o.getShape() == tmx::Object::Shape::Ellipse || o.getShape() == tmx::Object::Shape::Rectangle)
			{
				//use getAABB() to get the bounding box for the object
				const tmx::FloatRect& aabb = o.getAABB();
				SDL_FRect r{};
				r.x = aabb.left;
				r.y = aabb.top;
				r.w = aabb.width;
				r.h = aabb.height;
				tileColliders.emplace_back(TileCollider{ r, tag });
			}
			else
			{
				vector<SDL_FPoint> tileColliderPoints = {};
				//use getPoints() the get a vector of points to draw a more complex shape
				//this means we need to add a different type of ColliderComponent as the regular one takes an SDL_Rect for the shape.
				for (const tmx::Vector2f& p : o.getPoints())
				{
					SDL_FPoint point = { p.x, p.y };
					tileColliderPoints.emplace_back(point);
				}
				tileColliders.emplace_back(TileCollider{ o.getPosition().x, o.getPosition().y, tileColliderPoints, tag });
			}
		}
	}
	return tileColliders;
}

void TiledMapManager::AddTile(int srcX, int srcY, float x, float y, string tilesetAssetId, int tileSize, int scaleFactor, vector<TileCollider> &colliders, bool debug) const
{
	Entity& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, x, y, tilesetAssetId, tileSize, scaleFactor);
	tile.AddGroup(Game::groupMapTiles);
	for (TileCollider& c : colliders)
	{
		Entity& box(manager.addEntity());
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

void TiledMapManager::AddObject(float x, float y, string objectAssetId, float objectWidth, float objectHeight, int scaleFactor, vector<TileCollider>& colliders, bool shoudlYSort, bool debug) const
{
	Entity& mapObject(manager.addEntity());
	mapObject.addComponent<TransformComponent>(x, y, objectWidth, objectHeight, scaleFactor);
	mapObject.addComponent<SpriteComponent>(objectAssetId, false);
	mapObject.AddGroup(Game::groupMapObjects);
	for (TileCollider& c : colliders)
	{
		if (c.getColliderType() == Rectangle)
		{
			Entity& box(manager.addEntity());
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
			mapObject.addComponent<PolygonTransformComponent>(posX, posY, c.getColliderPoints(), scaleFactor, 0);
			mapObject.addComponent<ColliderComponent>(c.getColliderTag(), ColliderType::Polygon);

			if (debug)
			{
				mapObject.addComponent<PolygonLinesComponent>();
			}
		}
	}
	if (shoudlYSort)
	{
		mapObject.addComponent<YsortingComponent>();
	}

}
