#pragma once
#include "GameState.hpp"
#include "../entity_component_system/System.hpp"


class FreeFall : public GameState {
public:
	FreeFall();

	void update(float _time, float _deltaTime) override;
	void draw(float _time, float _deltaTime) override;

	void onResume() override;
	void onPause() override;

	void newState() override;
	bool isFinished();

private:
	Entity entity;
	Entity entity2;
	System system;
	const graphics::Texture2D& texture;
	const graphics::Texture2D& texture2;
	const graphics::Mesh meshSphere;
	const graphics::Mesh meshCrate;
};
