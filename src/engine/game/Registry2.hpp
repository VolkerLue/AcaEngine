#pragma once
#include <any>
#include <optional>
#include <vector>
#include <unordered_map>

template<typename T>
concept component_type = std::movable<T> && std::is_trivially_destructible<T>::value;

struct Entity
{
	uint32_t id;
};

struct EntityRef
{
	Entity ent;
	uint32_t generation;
};

template<typename Component>
struct componentStruct {
	std::vector<int> sparse;
	std::vector<Entity> entities;
	std::vector<Component> components;
};

class Registry2
{
public:

	Entity create();
	void erase(Entity _ent);

	EntityRef getRef(Entity _ent) const;
	std::optional<Entity> getEntity(EntityRef _ent) const;

	// Add a new component to an existing entity. No changes are done if Component
	// if_ent already has a component of this type.
	// @return A reference to the new component or the already existing component.
	template<component_type Component, typename... Args>
	Component& addComponent(Entity _ent, Args&&... _args);		

	// Remove a component from an existing entity.
	// Does not check whether it exists.
	template<component_type Component>
	void removeComponent(Entity _ent);

	// Retrieve a component associated with an entity.
	// @return The component or nullptr if the entity has no such component.
	template<component_type Component>
	Component* getComponent(Entity _ent);
	template<component_type Component>
	const Component* getComponent(Entity _ent) const;

	// Retrieve a component associated with an entity.
	// Does not check whether it exits.
	template<component_type Component>
	Component& getComponentUnsafe(Entity _ent);
	template<component_type Component>
	const Component& getComponentUnsafe(Entity _ent) const;

	// Execute an Action on all entities having the components
	// expected by Action::operator(component_type&...).
	// In addition, the entity itself is provided if
	// the first parameter is of type Entity.
	template<typename... Args, typename Action>
	void execute(const Action& _action);
	//template<typename Component, typename... Args, typename Action>
	//void func(Entity ent, const Action& action);
	template<typename Component, typename... Args, typename Action, typename Tuple>
	void func(Entity ent, const Action& action, Tuple tuple);

	std::vector<bool> flags;
	std::vector<uint32_t> generations;
	int numEntitys = 0;
	std::unordered_map<std::string, componentStruct<std::any>> componentMap;
	std::vector<std::string> co;
};
