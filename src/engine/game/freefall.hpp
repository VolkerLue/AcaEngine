#pragma once
#include "gamestate.hpp"
#include "../entity_component_system/system.hpp"
#include "../gui_toolkit/entity_creation_interface.hpp"


class FreeFall : public GameState {
public:
	FreeFall();

	void update(float _time, float _deltaTime) override;
	void draw(float _time, float _deltaTime) override;

	void onResume() override;
	void onPause(float _time) override;

	void newState() override;
	bool isFinished();

private:
	Entity entity;
	Entity entity2;
	System system;
	float timePaused;
};
