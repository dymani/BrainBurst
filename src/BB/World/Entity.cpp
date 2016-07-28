#include "BB/World/Entity.h"
#include "BB/World/World.h"

namespace bb {
  Entity::Entity(World& world, int id) : m_world(world), ID(id) {
  }

  Entity::~Entity() {
    for (auto& component : m_componentList) {
      m_world.getField().getSystem(component)->removeComponent(ID);
    }
  }

  void Entity::addComponent(IComponent* component) {
    m_componentList.push_back(component->TYPE);
    m_world.getField().getSystem(component->TYPE)->addComponent(component, ID);
  }

  void Entity::removeComponent(std::type_index type) {
    for (unsigned int i = 0; i < m_componentList.size();) {
      if (m_componentList[i] == type)
        m_componentList.erase(m_componentList.begin() + i);
      else
        i++;
    }
    m_world.getField().getSystem(type)->removeComponent(ID);
  }

  void Entity::markDirty() {
    m_dirty = true;
  }

  bool Entity::isDirty() {
    return m_dirty;
  }
}