#pragma once
#include "GameState.hpp"
#include "../systems/System.hpp"
#include <stdlib.h>
#include <time.h> 


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
	System system;
	const graphics::Texture2D& texturePlanet;
	const graphics::Texture2D& textureCratetex;
	const graphics::Mesh meshSphere;
	const graphics::Mesh meshCrate;
	int shot;
};
