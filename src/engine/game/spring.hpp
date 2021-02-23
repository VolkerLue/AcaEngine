#pragma once
#include "gamestate.hpp"
#include "../entity_component_system/system.hpp"


class Spring : public GameState {
public:
	Spring();

	void update(float _time, float _deltaTime) override;
	void draw(float _time, float _deltaTime) override;

	void onResume() override;
	void onPause() override;

	void newState() override;
	bool isFinished();

private:
	Entity entity;
	System system;
	const graphics::Texture2D& texture;
	const graphics::Mesh meshSphere;
};
