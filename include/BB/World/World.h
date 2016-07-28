#ifndef WORLD_H
#define WORLD_H

#include <typeindex>
#include <memory>
#include <SFML/Graphics.hpp>
#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <map>
#include <unordered_map>
#include "BB/World/System/ISystem.h"
#include "BB/World/Entity.h"
#include "BB/World/EntityTemplate.h"

namespace bb {
  class ResourceHandler;
  class WindowHandler;

  template<typename T>
  static std::type_index getType() {
    return std::type_index(typeid(T));
  }

  class World {
  public:
    World(ResourceHandler& resourceHandler, WindowHandler& windowHandler, luabridge::lua_State* L);
    void init();
    void load(std::string name);
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
    std::unordered_map<std::type_index, std::unique_ptr<ISystem>> m_systems;
    std::map<std::string, std::unique_ptr<EntityTemplate>> m_entityTemplates;
    std::map<int, std::unique_ptr<Entity>> m_entities;
  };
}

#endif