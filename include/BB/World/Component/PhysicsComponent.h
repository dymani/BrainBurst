#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "BB/World/Component/IComponent.h"

namespace bb {
	class PhysicsComponent : public IComponent {
		friend class PhysicsSystem;
	public:
		PhysicsComponent(int id) : IComponent(id, std::type_index(typeid(PhysicsComponent))) {
		}
		enum BodyType {
			STATIC, DYNAMIC, VIRTUAL
		};
	private:
		b2Body* m_body;
		sf::Vector2f m_size;
		BodyType m_bodyType;
	};
}

#endif