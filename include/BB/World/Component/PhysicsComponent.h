#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include <SFML/Graphics.hpp>
#include "BB/World/Component/IComponent.h"

namespace bb {
  class PhysicsComponent : public IComponent {
    friend class PhysicsSystem;
  public:
    PhysicsComponent(int id) : IComponent(id, std::type_index(typeid(PhysicsComponent))) {
    }
  private:
    sf::Vector2f m_size;
    sf::Vector2f m_coord;
  };
 }

#endif