#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include "Vector2D.hpp"


enum TileColliderType
{
	Rectangle,
	PointVector,
};

struct TileCollider
{
	private:
		Vector2D position;
		SDL_Rect colliderRect;
		std::string colliderTag;
		std::vector<SDL_Point> colliderPoints;
		TileColliderType colliderType;

	public:
		TileCollider(SDL_Rect rect, std::string tag);
		TileCollider(int x, int y, int w, int h, std::string tag);
		TileCollider(float x, float y, std::vector<SDL_Point> points, std::string tag);
		~TileCollider();
		Vector2D getColliderPosition() const;
		SDL_Rect getColliderRect() const;
		std::string getColliderTag();
		std::vector<SDL_Point> getColliderPoints();
		TileColliderType getColliderType() const;
};
