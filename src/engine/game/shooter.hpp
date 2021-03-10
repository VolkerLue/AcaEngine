#pragma once
#include "gamestate.hpp"
#include "../entity_component_system/system.hpp"
#include <stdlib.h>
#include <time.h> 
#include "../gui_toolkit/entity_creation_interface.hpp"


class Shooter : public GameState {
public:
	Shooter();

	void update(float _time, float _deltaTime) override;
	void draw(float _time, float _deltaTime) override;

	void onResume() override;
	void onPause(float _time) override;

	void newState() override;
	bool isFinished();

private:
	std::list<Entity> entities;
	System system;
	int shot;
	float timePaused;
};
