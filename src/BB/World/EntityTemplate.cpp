#include "BB/World/EntityTemplate.h"
#include "BB/World/World.h"
#include "BB/World/Component/PhysicsComponent.h"

namespace bb {
  EntityTemplate::EntityTemplate(World& world, luabridge::LuaRef luaE) : m_world(world) {
    IComponent* component;
    for (int j = 1; j <= luaE.length(); j++) {
      luabridge::LuaRef luaComponent = luaE[j];
      assert(luaComponent.isTable());
      luabridge::LuaRef luaType = luaComponent["type"];
      assert(luaType.isString());
      auto type = luaType.cast<std::string>();
      if (type == "Physics") {
        component = m_world.getSystem<PhysicsComponent>()->createTemplate(luaComponent);
        m_components.insert(std::make_pair(getType<PhysicsComponent>(),
          std::unique_ptr<IComponent>(component)));
      }
    }
  }

  Entity* EntityTemplate::createEntity(rapidjson::Value& jsonE) {
    using namespace rapidjson;
    Entity* entity;
    if (jsonE.HasMember("id")) {
      Value& jsonId = jsonE["id"];
      assert(jsonId.IsInt());
      entity = new Entity(m_world, jsonId.GetInt());
    }
    else {
      int id;
      do
        id = std::rand() % 1000;
      while (m_world.getEntity(id) != nullptr);
      entity = new Entity(m_world, id);
    }
    IComponent* c = nullptr;
    for (auto& component : m_components) {
      if (component.second->TYPE == getType<PhysicsComponent>())
        c = m_world.getSystem<PhysicsComponent>()->createComponent(component.second.get(), jsonE,
          entity->ID);
      else
        c = nullptr;
      assert(c != nullptr);
      entity->addComponent(c);
    }
    return entity;
  }

  EntityJsonGenerator::EntityJsonGenerator() : o(rapidjson::kObjectType) {
  }

  EntityJsonGenerator& EntityJsonGenerator::add(std::string name, int value) {
    using namespace rapidjson;
    Value v(value);
    Value index(name.c_str(), name.size(), d.GetAllocator());
    o.AddMember(index, v, d.GetAllocator());
    return *this;
  }

  EntityJsonGenerator& EntityJsonGenerator::add(std::string name, float value) {
    using namespace rapidjson;
    Value v(value);
    Value index(name.c_str(), name.size(), d.GetAllocator());
    o.AddMember(index, v, d.GetAllocator());
    return *this;
  }

  EntityJsonGenerator& EntityJsonGenerator::add(std::string name, std::string value) {
    using namespace rapidjson;
    Value v;
    v.SetString(StringRef(value.c_str()));
    Value index(name.c_str(), name.size(), d.GetAllocator());
    o.AddMember(index, v, d.GetAllocator());
    return *this;
  }

  EntityJsonGenerator& EntityJsonGenerator::add(std::string name, std::vector<int>& value) {
    using namespace rapidjson;
    Value a(kArrayType);
    for (auto v : value) {
      a.PushBack(v, d.GetAllocator());
    }
    Value index(name.c_str(), name.size(), d.GetAllocator());
    o.AddMember(index, a, d.GetAllocator());
    return *this;
  }

  EntityJsonGenerator& EntityJsonGenerator::add(std::string name, std::vector<float>& value) {
    using namespace rapidjson;
    Value a(kArrayType);
    for (auto v : value) {
      a.PushBack(v, d.GetAllocator());
    }
    Value index(name.c_str(), name.size(), d.GetAllocator());
    o.AddMember(index, a, d.GetAllocator());
    return *this;
  }

  EntityJsonGenerator& EntityJsonGenerator::add(std::string name, std::vector<std::string>& value) {
    using namespace rapidjson;
    Value a(kArrayType);
    for (auto v : value) {
      Value v2(v.c_str(), v.size(), d.GetAllocator());
      a.PushBack(v2, d.GetAllocator());
    }
    Value index(name.c_str(), name.size(), d.GetAllocator());
    o.AddMember(index, a, d.GetAllocator());
    return *this;
  }

  rapidjson::Value& EntityJsonGenerator::generateJson() {
    return o;
  }
}