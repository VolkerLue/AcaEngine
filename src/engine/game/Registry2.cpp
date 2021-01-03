#include <engine/game/Registry2.hpp>
#include <iostream>

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
		auto cS = it->second;
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
	//vector<string> comp = { (0, unpack_one<Args>()) ... };
	vector<string> comp = { typeid(Args).name() ... };
	if (comp[0] == typeid(Entity).name()) {
		comp.erase(comp.begin());
	}
	vector<Entity> ent;
	for (uint32_t en = 0; en < flags.size(); en++) {				//gets all entities that have all components
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
		//func<Args...>(*it, _action);
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


template<typename Component>
std::string unpack_one() {		
	return typeid(Component).name();
}

/*
template<typename Component, typename... Args, typename Action>
void Registry2::func(Entity ent, const Action& action) {
	if constexpr (std::is_same<Entity, Component>::value) {
		auto f = curry(action, ent);
		if constexpr (sizeof...(Args) > 0) func<Args...>(ent, f);
		else f();
	}
	else {
		Component& c = getComponentUnsafe<Component>(ent);
		auto f = curry(action, c);
		if constexpr (sizeof...(Args) > 0) func<Args...>(ent, f);
		else f();
	}
}

template<typename Function, typename... Args>
auto curry(Function function, Args&... args) {
	return [=](auto&... rest) {
		return function(args..., rest...);
	};
}*/
