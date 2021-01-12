#pragma once
#include "../game/Registry2.hpp"

class EntitySystem
{
public: 
	EntitySystem();

	Entity& createEntity(Entity& entity) {
		return entity = registry.create();
	}

	static Registry2 registry;
};