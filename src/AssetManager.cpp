#include "AssetManager.hpp"

AssetManager::AssetManager(Manager* man) : manager(man)
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::AddTexture(std::string id, const char* path)
{
	texturesMap.emplace(id, TextureManager::LoadTexture(path));
}

void AssetManager::AddTiledMap(std::string id, const char* path)
{
	tiledMapsMap.emplace(id, path);
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
	return texturesMap[id];
}

bool AssetManager::LoadtiledMap(std::string id, tmx::Map* tiledMap)
{
	return tiledMap->load(tiledMapsMap[id]);
}