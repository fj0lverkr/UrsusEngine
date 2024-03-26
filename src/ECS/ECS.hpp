#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <SDL2/SDL.h>

#include "EventSystem/EventReceiver.hpp"

class Component;
class Entity;
class Manager;

using ComponentId = std::size_t;
using Group = std::size_t;

inline ComponentId getNewComponentTypeId()
{
    static ComponentId lastId = 0u;
    return lastId++;
};

template <typename T>
inline ComponentId getComponentTypeId() noexcept
{
    static ComponentId typeId = getNewComponentTypeId();
    return typeId;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitset = std::bitset<maxComponents>;
using ComponentArray = std::array<Component *, maxComponents>;

using GroupBitset = std::bitset<maxGroups>;

class Component
{
private:
    std::vector<EventReceiver*> subscribers;
public:
    Entity* entity = {};

    virtual void init(){};
    virtual void update(){};
    virtual void draw(){};
    virtual ~Component(){};

    //Event handling by components
    bool HandleEvent(const SDL_Event* e)
    {
        for (const auto handler : subscribers)
        {
            if (handler->handleEvent(e))
            {
                return true;
            }
        }
        return false;
    };

    void SubscribeToEvents(EventReceiver* receiver)
    {
        subscribers.push_back(receiver);
    }
};

class Entity
{
private:
    Manager &manager;
    bool active = true;
    std::vector<std::unique_ptr<Component>> components;
    ComponentArray componentArray = {};
    ComponentBitset componentBitset;
    GroupBitset groupBitset;
    std::map<std::string, float> ySortValues = { {"MIN", -1.0f}, {"MAX", -1.0f} };

public:
    Entity(Manager &mMananger) : manager(mMananger) {}

    void update()
    {
        for (auto &c : components)
            c->update();
    }

    void draw()
    {
        for (auto &c : components)
            c->draw();
    }

    bool isActive() const { return active; }

    void destroy() { active = false; }

    bool hasGroup(Group mGroup)
    {
        return groupBitset[mGroup];
    }

    void AddGroup(Group mGroup);

    void DelGroup(Group mGroup)
    {
        groupBitset[mGroup] = false;
    }

    template <typename T>
    bool hasComponent() const
    {
        return componentBitset[getComponentTypeId<T>()];
    }

    template <typename T, typename... TArgs>
    T &addComponent(TArgs &&...mArgs)
    {
        T *c(new T(std::forward<TArgs>(mArgs)...));
        c->entity = this;
        std::unique_ptr<Component> uPtr{c};
        components.emplace_back(std::move(uPtr));

        componentArray[getComponentTypeId<T>()] = c;
        componentBitset[getComponentTypeId<T>()] = true;

        c->init();
        return *c;
    }

    template <typename T>
    T &GetComponent() const
    {
        auto ptr(componentArray[getComponentTypeId<T>()]);
        return *static_cast<T *>(ptr);
    }

    std::map<std::string, float> GetYSortValue() const
    {
        return ySortValues;
    }

    void SetMinYSortValue(float ySortVal)
    {
        ySortValues["MIN"] = ySortVal;
    }

    void SetMaxYSortValue(float ySortVal)
    {
        ySortValues["MAX"] = ySortVal;
    }

    void SetYSortValues(float MinYSortVal, float MaxYSortVal)
    {
        ySortValues["MIN"] = MinYSortVal;
        ySortValues["MAX"] = MaxYSortVal;
    }
};

class Manager
{
private:
  std::vector<std::unique_ptr<Entity>> entities;
  std::array<std::vector<Entity *>, maxGroups> groupedEntities;

public:
  void update()
  {
    for (auto &e : entities)
      e->update();
  }

  void draw()
  {
    for (auto &e : entities)
      e->draw();
  }

  void refresh()
  {
    for (auto i(0u); i < maxGroups; i++)
    {
      auto &v(groupedEntities[i]);
      v.erase(std::remove_if(std::begin(v), std::end(v), [i](Entity *mEntity)
        { return !mEntity->isActive() || !mEntity->hasGroup(i); }),
        std::end(v));
    }

    entities.erase(std::remove_if(std::begin(entities), std::end(entities), [](const std::unique_ptr<Entity> &mEntity)
        { return !mEntity->isActive(); }),
        std::end(entities));
}

  void addToGroup(Entity *mEntity, Group mGroup)
  {
    groupedEntities[mGroup].emplace_back(mEntity);
  }

  std::vector<Entity *> &getGroup(Group mGroup)
  {
    return groupedEntities[mGroup];
  }

  Entity &addEntity()
  {
    Entity *e = new Entity(*this);
    std::unique_ptr<Entity> uPtr(e);
    entities.emplace_back(std::move(uPtr));
    return *e;
  }
};
