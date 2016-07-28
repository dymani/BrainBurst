#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <memory>
#include <typeindex>
#include "BB/World/Component/IComponent.h"

namespace bb {
  class World;

  class Entity {
  public:
    const int ID;
    Entity(World& world, int id);
    ~Entity();
    void addComponent(IComponent* component);
    void removeComponent(std::type_index type);
    template<typename T>
    void removeComponent() {
      removeComponent(std::type_index(typeid(T)));
    }
    template<typename T>
    bool contains() {
      auto type = std::type_index(typeid(T));
      for (auto& component : m_components) {
        if (component == type)
          return true;
      }
      return false;
    }
    void markDirty();
    bool isDirty();
  private:
    World& m_world;
    std::vector<std::type_index> m_componentList;
    bool m_dirty;
  };
}

#endif