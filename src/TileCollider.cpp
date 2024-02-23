#include "TileCollider.hpp"

TileCollider::TileCollider(SDL_FRect rect, std::string tag)
{
	float fx = rect.x;
	float fy = rect.y;
	position = { fx, fy };
	colliderRect = rect;
	colliderTag = tag;
	colliderType = Rectangle;
}

TileCollider::TileCollider(float x, float y, float w, float h, std::string tag)
{
	float fx = x;
	float fy = y;
	position = { fx, fy };
	colliderRect = { x, y, w, h };
	colliderTag = tag;
	colliderType = Rectangle;
}

TileCollider::TileCollider(float x, float y, std::vector<SDL_FPoint> points, std::string tag)
{
	position = { x, y };
	colliderRect = { 0, 0, 0, 0 };
	colliderPoints = points;
	colliderTag = tag;
	colliderType = PointVector;
}

TileCollider::~TileCollider()
{
}

SDL_FRect TileCollider::getColliderRect() const
{
	return colliderRect;
}

std::string TileCollider::getColliderTag()
{
	return colliderTag;
}

std::vector<SDL_FPoint> TileCollider::getColliderPoints()
{
	return colliderPoints;
}

Vector2D TileCollider::getColliderPosition() const
{
	return position;
}

TileColliderType TileCollider::getColliderType() const
{
	return colliderType;
}
