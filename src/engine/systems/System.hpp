#pragma once
#include "EntitySystem.hpp"
#include "DrawSystem.hpp"
#include "PhysicSystem.hpp"


class System : public EntitySystem, public DrawSystem, public PhysicSystem
{
	
};