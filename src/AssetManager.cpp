#include "AssetManager.hpp"
#include "Util/StringUtils.hpp"

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

void AssetManager::AddTiledMap(std::string id, const std::string path)
{
	tiledMapsMap.emplace(id, path);
}

void AssetManager::AddTiledMaps(const char* p)
{
	using recursiveDirectoryIterator = std::filesystem::recursive_directory_iterator;
	for (const auto& entry : recursiveDirectoryIterator(p))
	{
		if (entry.is_regular_file())
		{
			const auto& path = entry.path();
			const auto& extension = path.extension().string();
			if (extension == ".tmx")
			{
				const auto& name = path.filename().string().substr(0, path.filename().string().size() - 4);
				std::string filePath = path.string();
				filePath = StringUtils::ReplaceAll(filePath, std::string("\\"), std::string("/"));
				AddTiledMap(name, filePath);
			}
		}
	}
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
	return texturesMap[id];
}

bool AssetManager::LoadtiledMap(std::string id, tmx::Map* tiledMap)
{
	return tiledMap->load(tiledMapsMap[id]);
}
