#include "BB/World/World.h"
#include "BB/Handler/ResourceHandler.h"
#include "BB/Handler/WindowHandler.h"

namespace bb {
  World::World(ResourceHandler& resourceHandler, WindowHandler& windowHandler, luabridge::lua_State* L)
    : m_resourceHandler(resourceHandler), m_windowHandler(windowHandler) {
    this->L = L;
    std::srand(unsigned int(std::time(0)));
    m_field = std::unique_ptr<Field>(new Field(m_resourceHandler, m_windowHandler, L, *this));
  }

  void World::init() {
    m_field->init();

    using namespace luabridge;
    assert(!luaL_loadfile(L, "assets/data/world/world.lua") && !lua_pcall(L, 0, 0, 0));

    luabridge::LuaRef luaEntityTemplates = getGlobal(L, "entityTemplates");
    assert(luaEntityTemplates.isTable());
    for (int i = 1; i <= luaEntityTemplates.length(); i++) {
      luabridge::LuaRef luaEntityTemplate = luaEntityTemplates[i];
      assert(luaEntityTemplate.isTable());
      luabridge::LuaRef luaName = luaEntityTemplate["name"];
      luabridge::LuaRef luaComponents = luaEntityTemplate["components"];
      assert(luaName.isString());
      assert(luaComponents.isTable());
      std::string name = luaName.cast<std::string>();
      auto* temp = new EntityTemplate(*this, luaComponents);
      m_entityTemplates.insert(std::make_pair(name, std::unique_ptr<EntityTemplate>(temp)));
    }
  }

  void World::load(std::string name, std::string field) {
    m_field->load(name, field);
  }

  void World::handleInput(sf::Event& windowEvent) {
    m_field->handleInput(windowEvent);
  }

  bool World::update() {
    bool willQuit = m_field->update();
    return willQuit;
  }

  void World::draw(const double dt) {
    m_field->draw(dt);
  }

  Field& World::getField() {
    return *(m_field.get());
  }
  EntityTemplate* World::getEntityTemplate(std::string name) {
    if (m_entityTemplates.find(name) != m_entityTemplates.end())
      return m_entityTemplates[name].get();
    return nullptr;
  }
}