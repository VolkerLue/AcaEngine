#pragma once
#include "../graphics/renderer/meshrenderer.hpp"
#include "../graphics/core/texture.hpp"
#include "../graphics/renderer/mesh.hpp"
#include "../graphics/camera.hpp"

class GameState {
public:
	GameState() : 
		finished(false) {}


	//has pure virtual functions for frame updates
	virtual void update(float _time, float _deltaTime) = 0;
	virtual void draw(float _time, float _deltaTime) = 0;

	//has virtual interface for state transition events
	virtual void onResume() = 0;
	virtual void onPause() = 0;

	//interface to mark current state as finished and to provide a new state
	virtual void newState() = 0;
	bool isFinished() {
		return finished;
	}

	bool finished;
};
