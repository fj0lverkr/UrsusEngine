#include "ECS.hpp"

void Entity::AddGroup(Group mGroup)
{
    groupBitset[mGroup] = true;
    manager.addToGroup(this, mGroup);
}