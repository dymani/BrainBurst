#ifndef I_COMPONENT_H
#define I_COMPONENT_H

#include <typeindex>

namespace bb {
  class IComponent {
  public:
    const int ID;
    const std::type_index TYPE;
		const bool IS_VIRTUAL;
    IComponent(int id, std::type_index type) : ID(id), TYPE(type), IS_VIRTUAL(ID == -1) {
    }
    virtual ~IComponent() {
    }
  protected:
  };
}

#endif