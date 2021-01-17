#pragma once
#include "GameState.hpp"
#include "../systems/System2.hpp"


class Shooter : public GameState {
public:
	Shooter();

	void update(float _time, float _deltaTime) override;
	void draw(float _time, float _deltaTime) override;

	void onResume() override;
	void onPause() override;

	void newState() override;
	bool isFinished();

private:
	std::list<Entity> entities;
	System2 system;
	const graphics::Texture2D& texture;
	const graphics::Texture2D& texture2;
};