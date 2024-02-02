#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <memory>
#include <vector>

class Component;
class Entity;

using ComponentId = std::size_t;

inline ComponentId getComponentTypeId() {
  static ComponentId lastId = 0;
  return lastId++;
};

template <typename T> inline ComponentId getComponentTypeId() noexcept {
  static ComponentId typeId = getComponentTypeId();
  return typeId;
}

constexpr std::size_t maxComponents = 32;

using ComponentBitset = std::bitset<maxComponents>;
using ComponentArray = std::array<Component *, maxComponents>;

class Component {
public:
  Entity *entity;

  virtual void init(){};
  virtual void update(){};
  virtual void draw(){};

  virtual ~Component(){};
};

class Entity {
private:
  bool active = true;
  std::vector<std::unique_ptr<Component>> components;
  ComponentArray componentArray;
  ComponentBitset componentBitset;

public:
  void update() {
    for (auto &c : components)
      c->update();
    for (auto &c : components)
      c->draw();
  }

  void draw() {}

  bool isActive() const { return active; }

  void destroy() { active = false; }

  template <typename T> bool hasComponent() const {
    return componentBitset[getComponentTypeId<T>];
  }

  template <typename T, typename... TArgs> T &addComponent(TArgs &&...mArgs) {
    T *c(new T(std::forward<TArgs>(mArgs)...));
    c->entity = this;
    std::unique_ptr<Component> uPtr{c};
    components.emplace_back(std::move(uPtr));

    componentArray[getComponentTypeId<T>()] = c;
    componentBitset[getComponentTypeId<T>()] = true;

    c->init();
    return *c;
  }

  template <typename T> T &GetComponent() const {
    auto ptr(componentArray[getComponentTypeId<T>()]);
    return *static_cast<T *>(ptr);
  }
};

class Manager {};
