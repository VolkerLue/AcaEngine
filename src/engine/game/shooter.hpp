#pragma once
#include "gamestate.hpp"
#include "../entity_component_system/system.hpp"
#include <stdlib.h>
#include <time.h> 
#include "../gui_toolkit/entity_creation_interface.hpp"
#include "../gui_toolkit/guitoolkit.hpp"
#include <engine/game/gamehelper.hpp>


class Shooter : public GameState {
public:
	Shooter();

	void update(float _time, float _deltaTime) override;
	void draw(float _time, float _deltaTime) override;

	void onResume() override;
	void onPause(float _time) override;

	void newState() override;
	bool isFinished();

private:
	const graphics::Texture2D& darkBlueTexture;
	const graphics::Texture2D& lightBlueTexture;
	std::list<Entity> entities;
	Entity menuButton;
	Entity checkBox;
	System system;
	GuiToolkit guiToolkit;
	std::string menu;
	std::string checkBoxText;
	int shot;
	float timePaused;
};
