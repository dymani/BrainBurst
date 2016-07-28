#ifndef I_COMPONENT_H
#define I_COMPONENT_H

#include <typeindex>

namespace bb {
  class IComponent {
  public:
    const int ID;
    const std::type_index TYPE;
    IComponent(int id, std::type_index type) : ID(id), TYPE(type) {
    }
    virtual ~IComponent() {
    }
  protected:
  };
}

#endif