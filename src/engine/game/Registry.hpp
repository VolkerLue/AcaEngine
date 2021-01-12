/*#pragma once
#include <cstdint>
#include <optional>
#include <vector>

struct Entity
{
	uint32_t id;
};
struct EntityRef
{
	Entity ent;
	uint32_t generation;
};
template<typename T>
class Registry
{
public:
	Entity create();
	void erase(Entity _ent);
	EntityRef getRef(Entity _ent) const;
	std::optional<Entity> getEntity(EntityRef _ent) const;

	void setData(Entity _ent, const T& _value);
	const T& getData(Entity _ent) const;
	T& getData(Entity _ent);

	template<typename FN>
	void execute(FN _fn);

	std::vector<T> data;
	std::vector<bool> flags;
	std::vector<uint32_t> generations;
	int numEntitys = 0;
};

*/
