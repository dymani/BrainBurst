#include "BB/World/System/PhysicsSystem.h"
#include <iostream>
#include "BB/World/World.h"

namespace bb {
	PhysicsSystem::PhysicsSystem(World& world) : m_world(world) {
	}

	IComponent* PhysicsSystem::createTemplate(luabridge::LuaRef luaC) {
		auto* c = new PhysicsComponent(-1);
		using namespace luabridge;
		luabridge::LuaRef luaSize = luaC["size"];
		luabridge::LuaRef luaBodyType = luaC["bodyType"];
		assert(luaSize.isTable());
		assert(luaSize[1].isNumber());
		assert(luaSize[2].isNumber());
		assert(luaBodyType.isString());
		if (luaBodyType.cast<std::string>() == "static") {
			c->m_bodyType = c->STATIC;
		}
		else if (luaBodyType.cast<std::string>() == "dynamic") {
			c->m_bodyType = c->DYNAMIC;
		}
		else {
			assert(false);
			c->m_bodyType = c->STATIC;
		}
		c->m_size = {luaSize[1].cast<float>(), luaSize[2].cast<float>()};
		return c;
	}

	IComponent* PhysicsSystem::createComponent(IComponent* component, rapidjson::Value& jsonE, int id) {
		auto* temp = dynamic_cast<PhysicsComponent*>(component);
		assert(temp != nullptr);
		using namespace rapidjson;
		PhysicsComponent* c;
		if (jsonE.HasMember("id")) {
			Value& jsonId = jsonE["id"];
			assert(jsonId.IsNumber());
			assert(jsonId.GetInt() != -1);
			c = new PhysicsComponent(jsonId.GetInt());
		}
		else {
			assert(id != -1);
			c = new PhysicsComponent(id);
		}
		Value& jsonCoordX = jsonE["x"];
		Value& jsonCoordY = jsonE["y"];
		assert(jsonCoordX.IsNumber());
		assert(jsonCoordY.IsNumber());

		b2BodyDef bodyDef;
		b2PolygonShape dynamicBox;
		b2FixtureDef fixtureDef;
		switch (temp->m_bodyType) {
			case PhysicsComponent::STATIC:
				bodyDef.position.Set(jsonCoordX.GetFloat(), jsonCoordY.GetFloat());
				c->m_body = m_world.getField().getBox2DWorld().CreateBody(&bodyDef);
				dynamicBox.SetAsBox(temp->m_size.x / 2.0f, temp->m_size.y / 2.0f);
				fixtureDef.shape = &dynamicBox;
				fixtureDef.density = 0.0f;
				fixtureDef.friction = 0.0f;
				c->m_body->CreateFixture(&fixtureDef);
				break;
			case PhysicsComponent::DYNAMIC:
				bodyDef.type = b2_dynamicBody;
				bodyDef.position.Set(jsonCoordX.GetFloat(), jsonCoordY.GetFloat());
				bodyDef.fixedRotation = true;
				c->m_body = m_world.getField().getBox2DWorld().CreateBody(&bodyDef);
				dynamicBox.SetAsBox(temp->m_size.x / 2.0f, temp->m_size.y / 2.0f);
				fixtureDef.shape = &dynamicBox;
				fixtureDef.density = 30.0f;
				fixtureDef.friction = 0.3f;
				c->m_body->CreateFixture(&fixtureDef);
				break;
		}
		return c;
	}

	void PhysicsSystem::addComponent(IComponent* component, int id) {
		PhysicsComponent* c = dynamic_cast<PhysicsComponent*>(component);
		assert(c != nullptr);
		assert(id != -1);
		assert(m_components.find(id) == m_components.end());
		m_components.insert(std::make_pair(id, std::unique_ptr<IComponent>(component)));
	}

	void PhysicsSystem::removeComponent(int id) {
		assert(m_components.find(id) != m_components.end());
		m_components.erase(id);
	}

	void PhysicsSystem::handleInput(sf::Event& windowEvent) {
	}

	bool PhysicsSystem::update() {
		m_world.getField().getBox2DWorld().Step(timeStep, velocityIterations, positionIterations);
		m_world.getField().getBox2DWorld().ClearForces();
		PhysicsComponent* c;
		for (auto& component : m_components) {
			c = dynamic_cast<PhysicsComponent*>(component.second.get());
			std::cout << c->ID << " "
				<< " " << c->m_body->GetPosition().x << " " << c->m_body->GetPosition().y << std::endl;
		}
		std::cout << "-----------" << std::endl;
		return false;
	}

	void PhysicsSystem::draw(const double dt) {
	}
}