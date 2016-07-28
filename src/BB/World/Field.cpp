#include "BB/World/Field.h"
#include "BB/Handler/ResourceHandler.h"
#include "BB/Handler/WindowHandler.h"
#include "BB/World/World.h"
#include "BB/World/System/PhysicsSystem.h"
#include "BB/World/Component/PhysicsComponent.h"

namespace bb {
  Field::Field(ResourceHandler& resourceHandler, WindowHandler& windowHandler, luabridge::lua_State* L,
    World& world) : m_resourceHandler(resourceHandler), m_windowHandler(windowHandler), m_world(world) {
    this->L = L;
  }

  void Field::init() {
    m_systems.insert(std::make_pair(getType<PhysicsComponent>(),
      std::unique_ptr<ISystem>(new PhysicsSystem())));
  }

  void Field::load(std::string name, std::string field) {
    std::string file = "saves/" + name + "/" + field + ".json";
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
      auto* temp = m_world.getEntityTemplate(jsonName.GetString());
      assert(temp != nullptr);
      Entity* entity = temp->createEntity(jsonEntity);
      m_entities.insert(std::make_pair(entity->ID, std::unique_ptr<Entity>(entity)));
    }
  }

  void Field::handleInput(sf::Event& windowEvent) {
    if (windowEvent.type == sf::Event::KeyPressed) {
      if (windowEvent.key.code == sf::Keyboard::J) {
        if (!m_entities.empty())
          m_entities.begin()->second->markDirty();
      }
      else if (windowEvent.key.code == sf::Keyboard::K) {
        EntityJsonGenerator json;
        json.add("x", 10).add("y", 11);
        auto* temp = m_world.getEntityTemplate("test1");
        auto* entity = temp->createEntity(json.generateJson().Move());
        m_entities.insert(std::make_pair(entity->ID, std::unique_ptr<Entity>(entity)));
      }
    }
  }

  bool Field::update() {
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

  void Field::draw(const double dt) {
    for (auto& system : m_systems) {
      system.second->draw(dt);
    }
  }

  Entity* Field::getEntity(int id) {
    if (m_entities.find(id) != m_entities.end())
      return m_entities[id].get();
    return nullptr;
  }

  void Field::deleteEntity(int id) {
    m_entities[id]->markDirty();
  }

  ISystem* Field::getSystem(std::type_index componentType) {
    return m_systems[componentType].get();
  }
}