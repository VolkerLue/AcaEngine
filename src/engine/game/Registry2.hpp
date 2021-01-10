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
};

#include "Registry2.hpp"


Entity Registry2::create() {
	uint32_t i;
	numEntitys++;
	for (i = 0; i < flags.size(); i++) {
		if (!flags[i]) {
			flags[i] = true;
			generations[i]++;
			return Entity{ i };
		}
	}
	flags.push_back(true);
	generations.push_back(1);
	return Entity{ i };
}


void Registry2::erase(Entity _ent) {
	flags[_ent.id] = false;
	numEntitys--;
	for (auto it = componentMap.begin(); it != componentMap.end(); it++) {
		auto& cS = it->second;
		if (cS.sparse[_ent.id] != -1) {
			int pos = cS.sparse[_ent.id];
			cS.sparse[_ent.id] = -1;
			cS.entities[pos] = cS.entities[cS.entities.size() - 1];
			cS.entities.pop_back();
			cS.components[pos] = cS.components[cS.components.size() - 1];
			cS.components.pop_back();
			cS.sparse[cS.entities[pos].id] = pos;
		}
	}
}


EntityRef Registry2::getRef(Entity _ent) const {
	return { _ent, generations[_ent.id] };
}


std::optional<Entity> Registry2::getEntity(EntityRef _ent) const {
	if (_ent.generation == generations[_ent.ent.id] && flags[_ent.ent.id]) {
		return _ent.ent;
	}
	return std::nullopt;
}


template<component_type Component, typename... Args>
Component& Registry2::addComponent(Entity _ent, Args&&... _args) {
	if (componentMap.contains(typeid(Component).name())) {
		auto& cS = componentMap[typeid(Component).name()];
		if (cS.sparse[_ent.id] != -1) {
			return std::any_cast<Component&>(cS.components[cS.sparse[_ent.id]]);
		}
	}
	else {
		std::vector<int> sparse(100, -1);
		std::vector<Entity> entities;
		std::vector<std::any> components;
		componentStruct<std::any> comStr = { sparse, entities, components };
		componentMap[typeid(Component).name()] = comStr;
	}
	auto& cS = componentMap[typeid(Component).name()];
	cS.sparse[_ent.id] = cS.entities.size();
	cS.entities.push_back(_ent);
	cS.components.push_back(std::any(Component(_args...)));
	return std::any_cast<Component&>(cS.components[cS.sparse[_ent.id]]);
}


template<component_type Component>
void Registry2::removeComponent(Entity _ent) {
	if (componentMap.contains(typeid(Component).name())) {
		auto& cS = componentMap[typeid(Component).name()];
		if (cS.sparse[_ent.id] != -1) {
			int pos = cS.sparse[_ent.id];
			cS.sparse[_ent.id] = -1;
			cS.entities[pos] = cS.entities[cS.entities.size() - 1];
			cS.entities.pop_back();
			cS.components[pos] = cS.components[cS.components.size() - 1];
			cS.components.pop_back();
			cS.sparse[cS.entities[pos].id] = pos;
		}
		if (cS.entities.size() == 0) {
			componentMap.erase(typeid(Component).name());
		}
	}
}


template<component_type Component>
Component* Registry2::getComponent(Entity _ent) {
	if (componentMap.contains(typeid(Component).name())) {
		auto& cS = componentMap[typeid(Component).name()];
		if (cS.sparse[_ent.id] != -1) {
			return std::any_cast<Component>(&cS.components[cS.sparse[_ent.id]]);
		}
	}
	return nullptr;
}


template<component_type Component>
const Component* Registry2::getComponent(Entity _ent) const {
	if (componentMap.contains(typeid(Component).name())) {
		auto& cS = componentMap[typeid(Component).name()];
		if (cS.sparse[_ent.id] != -1) {
			return &cS.components[cS.sparse[_ent.id]];
		}
	}
	return nullptr;
}


template<component_type Component>
Component& Registry2::getComponentUnsafe(Entity _ent) {
	auto& cS = componentMap[typeid(Component).name()];
	return std::any_cast<Component&>(cS.components[cS.sparse[_ent.id]]);
}


template<component_type Component>
const Component& Registry2::getComponentUnsafe(Entity _ent) const {
	auto& cS = componentMap[typeid(Component).name()];
	return std::any_cast<Component&>(cS.components[cS.sparse[_ent.id]]);
}


template<typename... Args, typename Action>
void Registry2::execute(const Action& _action) {
	using namespace std;
	bool hasAllComponents;
	vector<string> comp = { (0, typeid(Args).name()) ... };
	if (comp[0] == typeid(Entity).name()) {
		comp.erase(comp.begin());
	}
	vector<Entity> ent;
	for (uint32_t en = 0; en < flags.size(); en++) {				//gets all the entities that have all components
		if (!flags[en]) continue;
		hasAllComponents = true;
		for (auto it = comp.begin(); it != comp.end(); it++) {		//checks if entity has all components
			if (componentMap[*it].sparse[en] == -1) {
				hasAllComponents = false;
				break;
			}
		}
		if (hasAllComponents) {
			ent.push_back(Entity(en));
		}
	}
	for (auto it = ent.begin(); it != ent.end(); it++) {
		func<Args...>(*it, _action, std::tie());
	}
}


template<typename Component, typename... Args, typename Action, typename Tuple>
void Registry2::func(Entity ent, const Action& action, Tuple tuple) {
	if constexpr (std::is_same<Entity, Component>::value) {
		auto tu = std::tuple_cat(tuple, std::tie(ent));
		if constexpr (sizeof...(Args) > 0) func<Args...>(ent, action, tu);
		else std::apply(action, tu);
	}
	else {
		Component& c = getComponentUnsafe<Component>(ent);
		auto tu = std::tuple_cat(tuple, std::tie(c));
		if constexpr (sizeof...(Args) > 0) func<Args...>(ent, action, tu);
		else std::apply(action, tu);
	}
}
