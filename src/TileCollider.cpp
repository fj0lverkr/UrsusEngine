#include "TileCollider.hpp"

TileCollider::TileCollider(SDL_Rect rect, std::string tag)
{
	colliderRect = rect;
	colliderTag = tag;
	colliderType = Rectangle;
}

TileCollider::TileCollider(int x, int y, int w, int h, std::string tag)
{
	colliderRect = { x, y, w, h };
	colliderTag = tag;
	colliderType = Rectangle;
}

TileCollider::TileCollider(std::vector<SDL_Point> points, std::string tag)
{
	colliderPoints = points;
	colliderTag = tag;
	colliderType = PointVector;
}

TileCollider::~TileCollider()
{
}

SDL_Rect TileCollider::getColliderRect()
{
	return colliderRect;
}

std::string TileCollider::getColliderTag()
{
	return colliderTag;
}

std::vector<SDL_Point> TileCollider::getColliderPoints()
{
	return colliderPoints;
}

TileColliderType TileCollider::getColliderType()
{
	return colliderType;
}
