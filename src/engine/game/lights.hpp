#pragma once
#include "GameState.hpp"
#include <engine/entity_component_system/system.hpp>
#include <engine/gui_toolkit/entity_creation_interface.hpp>
#include <engine/gui_toolkit/guitoolkit.hpp>
#include <stdlib.h>
#include <time.h> 
#include <engine/game/gamehelper.hpp>


class Lights : public GameState {
public:
	Lights();

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
	std::vector<Entity> lightEntities;
	std::vector<Entity> checkBoxes;
	std::vector<std::string> checkBoxTexts;
	Entity meshEntity;
	Entity menuButton;
	Entity textDisplay;
	Entity container;
	System system;
	GuiToolkit guiToolkit;
	std::string menu;
	std::string textDisplayText;
	std::string checkBoxText1;
	std::string checkBoxText2;
	std::string checkBoxText3;
	std::string checkBoxText4;
	float timePaused;
};