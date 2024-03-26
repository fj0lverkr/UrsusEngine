#pragma once
#include "ECS.hpp"
#include "Components.hpp"

class YsortingComponent : public Component
{
private:
	AnchorComponent* anchor = nullptr;
	ColliderComponent* collider = nullptr;

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
			collider = &entity->GetComponent<ColliderComponent>();
		}
	}

	void update() override
	{
		if (anchor != nullptr)
		{
			entity->SetYSortValues(anchor->transform->position.y, anchor->transform->position.y);
		}
		else if (collider != nullptr)
		{
			switch (collider->getType())
			{
			case ColliderType::AABB:
				break;
			case ColliderType::Polygon:
				break;
			default:
				break;
			}
		}
	}
};