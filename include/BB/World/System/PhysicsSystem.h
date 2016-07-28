#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <SFML/Graphics.hpp>
#include "BB/World/System/ISystem.h"
#include "BB/World/Component/PhysicsComponent.h"

namespace bb {
  class PhysicsSystem: public ISystem {
  public:
    PhysicsSystem();
    IComponent* createTemplate(luabridge::LuaRef luaC);
    IComponent* createComponent(IComponent* component, rapidjson::Value& jsonE, int id = -1);
    void addComponent(IComponent* component, int id);
    void removeComponent(int id);
    bool update();
  private:
  };
}

#endif