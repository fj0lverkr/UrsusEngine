#include "TileCollider.hpp"

TileCollider::TileCollider(SDL_Rect rect, std::string tag)
{
	float fx = static_cast<float>(rect.x);
	float fy = static_cast<float>(rect.y);
	position = { fx, fy };
	colliderRect = rect;
	colliderTag = tag;
	colliderType = Rectangle;
}

TileCollider::TileCollider(int x, int y, int w, int h, std::string tag)
{
	float fx = static_cast<float>(x);
	float fy = static_cast<float>(y);
	position = { fx, fy };
	colliderRect = { x, y, w, h };
	colliderTag = tag;
	colliderType = Rectangle;
}

TileCollider::TileCollider(float x, float y, std::vector<SDL_Point> points, std::string tag)
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

SDL_Rect TileCollider::getColliderRect() const
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

Vector2D TileCollider::getColliderPosition() const
{
	return position;
}

TileColliderType TileCollider::getColliderType() const
{
	return colliderType;
}
