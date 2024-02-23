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
		SDL_FRect colliderRect;
		std::string colliderTag;
		std::vector<SDL_FPoint> colliderPoints;
		TileColliderType colliderType;

	public:
		TileCollider(SDL_FRect rect, std::string tag);
		TileCollider(float x, float y, float w, float h, std::string tag);
		TileCollider(float x, float y, std::vector<SDL_FPoint> points, std::string tag);
		~TileCollider();
		Vector2D getColliderPosition() const;
		SDL_FRect getColliderRect() const;
		std::string getColliderTag();
		std::vector<SDL_FPoint> getColliderPoints();
		TileColliderType getColliderType() const;
};
