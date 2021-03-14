#pragma once
#include "gamestate.hpp"
#include "../entity_component_system/system.hpp"
#include "engine/gui_toolkit/entity_creation_interface.hpp"
#include "engine/gui_toolkit/guitoolkit.hpp"


class Spring : public GameState {
public:
	Spring();

	void update(float _time, float _deltaTime) override;
	void draw(float _time, float _deltaTime) override;

	void onResume() override;
	void onPause(float _time) override;

	void newState() override;
	bool isFinished();

private:
	const graphics::Texture2D& darkBlueTexture;
	const graphics::Texture2D& lightBlueTexture;
	const graphics::Texture2D& whiteTexture;
	int lastFactor;
	Entity entity;
	Entity sliderEntity;
	Entity menuButton;
	Entity textDisplay;
	System system;
	GuiToolkit guiToolkit;
	std::string menu;
	std::string textDisplayText;
	float timePaused;
};
