#pragma once
#include "Registry.hpp"
#include <iostream>

template<typename T>
Entity Registry<T>::create() {
	uint32_t i;
	numEntitys++;
	for (i = 0; i < flags.size(); i++) {
		if (!flags[i]) {
			flags[i] = true;
			generations[i]++;
			data[i] = (T) 0;
			return Entity{ i };
		}
	}
	flags.push_back(true);
	generations.push_back(1);
	data.push_back((T) 0);
	return Entity{ i };
}

template<typename T>
void Registry<T>::erase(Entity _ent) {
	flags[_ent.id] = false;
	data[_ent.id] = (T) 0;
	numEntitys--;
}

template<typename T>
EntityRef Registry<T>::getRef(Entity _ent) const {
	return { _ent, generations[_ent.id] };
}

template<typename T>
std::optional<Entity> Registry<T>::getEntity(EntityRef _ent) const {
	if (_ent.generation == generations[_ent.ent.id] && flags[_ent.ent.id]) {
		return _ent.ent;
	}
}

template<typename T>
void Registry<T>::setData(Entity _ent, const T& _value) {
	data[_ent.id] = _value;
}

template<typename T>
const T& Registry<T>::getData(Entity _ent) const {
	return data[_ent.id];
}

template<typename T>
T& Registry<T>::getData(Entity _ent) {
	return data[_ent.id];
}

template<typename T>
template<typename FN>
void Registry<T>::execute(FN _fn) {
	int found = 0;
	for (int i = 0; i < flags.size() && found < numEntitys; i++) {
		if (flags[i]) {
			++found;
			_fn(data[i]);
		}
	}
}

template<typename T>
struct Printer {
	void operator()(T& _data) const {
		std::cout << _data << ", ";
	}
};

template<typename T>
struct Add2 {
	void operator()(T& _data) const {
		_data += 2;
	}
};

template<typename T>
struct Multiply3 {
	void operator()(T& _data) const {
		_data *= 3;
	}
};

template<typename T>
struct Sum {
	Sum() {
		sum = 0;
	}
	int sum;
	void operator()(T& _data) {
		sum += _data;
		_data = sum;
	}
};
