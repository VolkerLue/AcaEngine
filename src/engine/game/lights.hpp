#pragma once
#include "GameState.hpp"
#include <engine/entity_component_system/system.hpp>
#include <engine/gui_toolkit/entity_creation_interface.hpp>
#include <stdlib.h>
#include <time.h> 


class Lights : public GameState {
public:
	Lights();

	void update(float _time, float _deltaTime) override;
	void draw(float _time, float _deltaTime) override;

	void onResume() override;
	void onPause(float _time) override;

	void newState() override;
	bool isFinished();

private:
	std::vector<Entity> lightEntities;
	Entity meshEntity;
	System system;
	float timePaused;
};
