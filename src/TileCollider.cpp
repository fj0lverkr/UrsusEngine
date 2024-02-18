#include "TileCollider.hpp"

TileCollider::TileCollider(SDL_Rect rect, std::string tag)
{
	colliderRect = rect;
	colliderTag = tag;
}

TileCollider::TileCollider(int x, int y, int w, int h, std::string tag)
{
	colliderRect = { x, y, w, h };
	colliderTag = tag;
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
