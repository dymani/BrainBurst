#include "BB/World/System/PhysicsSystem.h"
#include <iostream>

namespace bb {
  PhysicsSystem::PhysicsSystem() {
  }

  IComponent* PhysicsSystem::createTemplate(luabridge::LuaRef luaC) {
    auto* c = new PhysicsComponent(-1);
    using namespace luabridge;
    luabridge::LuaRef luaSize = luaC["size"];
    assert(luaSize.isTable());
    assert(luaSize[1].isNumber());
    assert(luaSize[2].isNumber());
    c->m_size = {luaSize[1].cast<float>(), luaSize[2].cast<float>()};
    return c;
  }

  IComponent* PhysicsSystem::createComponent(IComponent* component, rapidjson::Value& jsonE, int id) {
    PhysicsComponent* temp = dynamic_cast<PhysicsComponent*>(component);
    assert(temp != nullptr);
    using namespace rapidjson;
    PhysicsComponent* c;
    if (jsonE.HasMember("id")) {
      Value& jsonId = jsonE["id"];
      assert(jsonId.IsNumber());
      c = new PhysicsComponent(jsonId.GetInt());
    }
    else {
      assert(id != -1);
      c = new PhysicsComponent(id);
    }
    c->m_size = temp->m_size;
    Value& jsonCoordX = jsonE["x"];
    Value& jsonCoordY = jsonE["y"];
    assert(jsonCoordX.IsNumber());
    assert(jsonCoordY.IsNumber());
    c->m_coord = {jsonCoordX.GetFloat(), jsonCoordY.GetFloat()};
    return c;
  }

  void PhysicsSystem::addComponent(IComponent* component, int id) {
    PhysicsComponent* c = dynamic_cast<PhysicsComponent*>(component);
    assert(c != nullptr);
    assert(m_components.find(id) == m_components.end());
    m_components.insert(std::make_pair(id, std::unique_ptr<IComponent>(component)));
  }

  void PhysicsSystem::removeComponent(int id) {
    assert(m_components.find(id) != m_components.end());
    m_components.erase(id);
  }

  bool PhysicsSystem::update() {
    PhysicsComponent* c;
    for (auto& component : m_components) {
      c = dynamic_cast<PhysicsComponent*>(component.second.get());
      std::cout << c->ID << " " << c->m_size.x << " " << c->m_size.y
        << " " << c->m_coord.x << " " << c->m_coord.y << std::endl;
    }
    std::cout << "-----------" << std::endl;
    return false;
  }
}