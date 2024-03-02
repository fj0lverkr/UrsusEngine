#pragma once
#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <tmxlite/Map.hpp>
#include "TextureManager.hpp"
#include "Vector2D.hpp"
#include "ECS/ECS.hpp"

class AssetManager {
private:
	std::map<std::string, SDL_Texture*> texturesMap;
	std::map<std::string, const char*> tiledMapsMap;
	Manager* manager;
public:
	AssetManager(Manager* man);
	~AssetManager();
	void AddTexture(std::string id, const char* path);
	void AddTiledMap(std::string id, const char* path);
	SDL_Texture* GetTexture(std::string id);
	bool LoadtiledMap(std::string id, tmx::Map* tiledMap);
};