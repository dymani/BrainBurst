#ifndef FIELD_H
#define FIELD_H

#include <typeindex>
#include <memory>
#include <map>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include "BB/World/System/ISystem.h"
#include "BB/World/Entity.h"

namespace bb {
  class ResourceHandler;
  class WindowHandler;
  class World;

  class Field {
  public:
    Field(ResourceHandler& resourceHandler, WindowHandler& windowHandler, luabridge::lua_State* L,
      World& world);
    void init();
    void load(std::string name, std::string field);
    void handleInput(sf::Event& windowEvent);
    bool update();
    void draw(const double dt);
    Entity* getEntity(int id);
    void deleteEntity(int id);
    ISystem* getSystem(std::type_index componentType);
    template<typename T>
    ISystem* getSystem() {
      return getSystem(std::type_index(typeid(T)));
    }
  private:
    ResourceHandler& m_resourceHandler;
    WindowHandler& m_windowHandler;
    luabridge::lua_State* L;
    World& m_world;
    std::unordered_map<std::type_index, std::unique_ptr<ISystem>> m_systems;
    std::map<int, std::unique_ptr<Entity>> m_entities;
  };
}

#endif