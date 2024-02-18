#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <string>

struct TileCollider
{
	private:
		SDL_Rect colliderRect;
		std::string colliderTag;

	public:
		TileCollider(SDL_Rect rect, std::string tag);
		TileCollider(int x, int y, int w, int h, std::string tag);
		~TileCollider();
		SDL_Rect getColliderRect();
		std::string getColliderTag();
};