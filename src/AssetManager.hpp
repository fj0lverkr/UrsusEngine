#pragma once
#include <map>
#include <string>
#include <filesystem>
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

	// TODO if this works it should be moved to a utils class or something like that
	std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
		}
		return str;
	}
public:
	AssetManager(Manager* man);
	~AssetManager();
	void AddTexture(std::string id, const char* path);
	void AddTiledMap(std::string id, const char* path);
	void AddTiledMaps(const char* path);
	SDL_Texture* GetTexture(std::string id);
	bool LoadtiledMap(std::string id, tmx::Map* tiledMap);
};