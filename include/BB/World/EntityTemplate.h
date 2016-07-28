#ifndef ENTITY_TEMPLATE_H
#define ENTITY_TEMPLATE_H

#include <unordered_map>
#include <typeindex>
#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <rapidjson/document.h>
#include <memory>
#include "BB/World/Component/IComponent.h"
#include "BB/World/Entity.h"

namespace bb {
  class World;

  class EntityTemplate {
  public:
    EntityTemplate(World& world, luabridge::LuaRef luaE);
    Entity* createEntity(rapidjson::Value& jsonE);
  private:
    World& m_world;
    std::unordered_map<std::type_index, std::unique_ptr<IComponent>> m_components;
  };

  class EntityJsonGenerator {
  public:
    EntityJsonGenerator();
    EntityJsonGenerator& add(std::string name, int value);
    EntityJsonGenerator& add(std::string name, float value);
    EntityJsonGenerator& add(std::string name, std::string value);
    EntityJsonGenerator& add(std::string name, std::vector<int>& value);
    EntityJsonGenerator& add(std::string name, std::vector<float>& value);
    EntityJsonGenerator& add(std::string name, std::vector<std::string>& value);
    rapidjson::Value& generateJson();
  private:
    rapidjson::Document d;
    rapidjson::Value o;
  };
}

#endif