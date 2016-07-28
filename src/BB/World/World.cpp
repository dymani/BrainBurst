#include "BB/World/World.h"
#include "BB/Handler/ResourceHandler.h"
#include "BB/Handler/WindowHandler.h"
#include "BB/World/System/PhysicsSystem.h"
#include "BB/World/Component/PhysicsComponent.h"

namespace bb {
  World::World(ResourceHandler& resourceHandler, WindowHandler& windowHandler, luabridge::lua_State* L)
    : m_resourceHandler(resourceHandler), m_windowHandler(windowHandler) {
    this->L = L;
    std::srand(unsigned int(std::time(0)));
  }

  void World::init() { //load world assets and templates
    m_systems.insert(std::make_pair(getType<PhysicsComponent>(),
      std::unique_ptr<ISystem>(new PhysicsSystem())));

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

  void World::load(std::string name) { //load world save contents
    std::string file = "saves/" + name + ".json";
    std::ifstream fin(file);
    assert(!fin.fail());
    std::stringstream strStream;
    strStream << fin.rdbuf();
    std::string fileString = strStream.str();
    fin.close();
    using namespace rapidjson;
    Document document;
    assert(!document.Parse<0>(fileString.c_str()).HasParseError());
    Value& jsonEntities = document["entities"];
    for (SizeType i = 0; i < jsonEntities.Size(); i++) {
      Value& jsonEntity = jsonEntities[i];
      Value& jsonName = jsonEntity["name"];
      assert(m_entityTemplates.find(jsonName.GetString()) != m_entityTemplates.end());
      Entity* entity = m_entityTemplates[jsonName.GetString()]->createEntity(jsonEntity);
      m_entities.insert(std::make_pair(entity->ID, std::unique_ptr<Entity>(entity)));
    }
  }

  void World::handleInput(sf::Event& windowEvent) {
    if (windowEvent.type == sf::Event::KeyPressed) {
      if (windowEvent.key.code == sf::Keyboard::J) {
        if (!m_entities.empty())
          m_entities.begin()->second->markDirty();
      }
      else if (windowEvent.key.code == sf::Keyboard::K) {
        EntityJsonGenerator json;
        json.add("x", 10).add("y", 11);
        auto* entity = m_entityTemplates["test1"]->createEntity(json.generateJson().Move());
        m_entities.insert(std::make_pair(entity->ID, std::unique_ptr<Entity>(entity)));
      }
    }
  }

  bool World::update() {
    bool willQuit = false;
    for (auto& system : m_systems) {
      if (system.second->update())
        willQuit = true;
    }
    for (auto it = m_entities.cbegin(); it != m_entities.cend();) {
      if (it->second->isDirty())
        m_entities.erase(it++);
      else
        ++it;
    }
    return willQuit;
  }

  void World::draw(const double dt) {
  }

  Entity* World::getEntity(int id) {
    if (m_entities.find(id) != m_entities.end())
      return m_entities[id].get();
    return nullptr;
  }

  void World::deleteEntity(int id) {
    m_entities[id]->markDirty();
  }

  ISystem* World::getSystem(std::type_index componentType) {
    return m_systems[componentType].get();
  }
}