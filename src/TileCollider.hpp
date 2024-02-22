#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <string>


enum TileColliderType
{
	Rectangle,
	PointVector,
};

struct TileCollider
{
	private:
		SDL_Rect colliderRect;
		std::string colliderTag;
		std::vector<SDL_Point> colliderPoints;
		TileColliderType colliderType;

	public:
		TileCollider(SDL_Rect rect, std::string tag);
		TileCollider(int x, int y, int w, int h, std::string tag);
		TileCollider(std::vector<SDL_Point> points, std::string tag);
		~TileCollider();
		SDL_Rect getColliderRect();
		std::string getColliderTag();
		std::vector<SDL_Point> getColliderPoints();
		TileColliderType getColliderType();
};
