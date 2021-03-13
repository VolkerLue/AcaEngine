#pragma once
#include "gamestate.hpp"
#include "../entity_component_system/system.hpp"
#include "../gui_toolkit/entity_creation_interface.hpp"
#include "../gui_toolkit/guitoolkit.hpp"


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
	const graphics::Texture2D& darkBlueTexture;
	const graphics::Texture2D& lightBlueTexture;
	Entity entity;
	Entity entity2;
	Entity menuButton;
	System system;
	GuiToolkit guiToolkit;
	std::string menu;
	float timePaused;
};
