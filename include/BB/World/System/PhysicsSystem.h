#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <SFML/Graphics.hpp>
#include "BB/World/System/ISystem.h"
#include "BB/World/Component/PhysicsComponent.h"

namespace bb {
	class World;

	class PhysicsSystem : public ISystem {
	public:
		PhysicsSystem(World& world);
		IComponent* createTemplate(luabridge::LuaRef luaC);
		IComponent* createComponent(IComponent* component, rapidjson::Value& jsonE, int id = -1);
		void addComponent(IComponent* component, int id);
		void removeComponent(int id);
		void handleInput(sf::Event& windowEvent);
		bool update();
		void draw(const double dt);
	private:
		World& m_world;
		float32 timeStep = 1.0F / 50.0F;
		int32 velocityIterations = 8;
		int32 positionIterations = 3;
	};
}

#endif