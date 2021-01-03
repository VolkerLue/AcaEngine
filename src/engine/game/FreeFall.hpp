#pragma once
#include "GameState.hpp"
#include "Registry2.hpp"

class FreeFall : public GameState {
public:
	FreeFall();
	//has pure virtual functions for frame updates
	void update(float _time, float _deltaTime) override;
	void draw(float _time, float _deltaTime) override;

	//has virtual interface for state transition events
	void onResume() override;
	void onPause() override;

	//interface to mark current state as finished and to provide a new state
	void newState() override;
	bool isFinished();

private:
	const graphics::Texture2D& texture;
	graphics::Camera camera;
	graphics::MeshRenderer renderer;
	float a;
	float v;

	Registry2 registry;
	Entity planet1;
};
