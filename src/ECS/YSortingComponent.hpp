#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include "Components.hpp"

class YsortingComponent : public Component
{
private:
	AnchorComponent* anchor = nullptr;
	ColliderComponent* shape = nullptr;

public:
	YsortingComponent() {};
	~YsortingComponent() {};

	void init() override
	{
		if (entity->hasComponent<AnchorComponent>())
		{
			anchor = &entity->GetComponent<AnchorComponent>();
		}
		if (entity->hasComponent<ColliderComponent>())
		{
			shape = &entity->GetComponent<ColliderComponent>();
		}
	}

	void update() override
	{
		if (anchor != nullptr)
		{
			entity->SetYSortValues(anchor->collider.y, anchor->collider.y);
		}
		else if (shape != nullptr)
		{
			std::vector<SDL_FPoint> colliderPoints = shape->colliderPoints;
			float minY, maxY;
			minY = maxY = -1.0f;
			switch (shape->getType())
			{
			case AABB:
				minY = shape->collider.y;
				maxY = minY + shape->collider.h;
				break;
			case Polygon:
				minY = maxY = colliderPoints[0].y;
				for (auto& p : colliderPoints)
				{
					minY = p.y < minY ? p.y : minY;
					maxY = p.y > maxY ? p.y : maxY;
				}
				break;
			default:
				break;
			}

			entity->SetYSortValues(minY, maxY);
		}
	}
};