#pragma once
#include <map>
#include <string>
#include <filesystem>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <tmxlite/Map.hpp>
#include "TextureManager.hpp"
#include "Vector2D.hpp"
#include "ECS/ECS.hpp"

class AssetManager {
private:
	std::map<std::string, SDL_Texture*> texturesMap;
	std::map<std::string, const std::string> tiledMapsMap;
	std::map<std::string, TTF_Font*> fontsMap;
	Manager* manager;

public:
	AssetManager(Manager* man);
	~AssetManager();
	void AddTexture(std::string id, const char* path);
	void AddTiledMap(std::string id, const std::string path);
	void AddTiledMaps(const char* path);
	void AddFont(std::string id, const char* path, int fontSize);
	SDL_Texture* GetTexture(std::string id);
	bool LoadtiledMap(std::string id, tmx::Map* tiledMap);
	TTF_Font* GetFont(std::string id);
};