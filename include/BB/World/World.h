#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics.hpp>
#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <memory>
#include <map>
#include "BB/World/Field.h"
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
    void load(std::string name, std::string field);
    void handleInput(sf::Event& windowEvent);
    bool update();
    void draw(const double dt);
    Field& getField();
    EntityTemplate* getEntityTemplate(std::string name);
  private:
    ResourceHandler& m_resourceHandler;
    WindowHandler& m_windowHandler;
    luabridge::lua_State* L;
    std::unique_ptr<Field> m_field;
    std::map<std::string, std::unique_ptr<EntityTemplate>> m_entityTemplates;
  };
}

#endif