#ifndef GRAPHICS_COMPONENT
#define GRAPHICS_COMPONENT

#include <SFML/Graphics.hpp>
#include "BB/World/Component/IComponent.h"

namespace bb {
  class GraphicsComponent : public IComponent {
  public:
    GraphicsComponent(int id) : IComponent(id, std::type_index(typeid(GraphicsComponent))) {
    }
  private:
    sf::Sprite m_sprite;
  };
}

#endif