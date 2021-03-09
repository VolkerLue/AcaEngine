#pragma once
#include "gamestate.hpp"
#include "../gui_toolkit/guitoolkit.hpp"

class ButtonDemo : public GameState {
public:
	ButtonDemo();

	void update(float _time, float _deltaTime) override;
	void draw(float _time, float _deltaTime) override;

	void onResume() override;
	void onPause(float _time) override;

	void newState() override;
	bool isFinished();

private:
	System system;
	GuiToolkit guiToolkit;

	const graphics::Texture2D& darkBlueTexture;	
	const graphics::Texture2D& lightBlueTexture;
	const graphics::Texture2D& whiteTexture;
	const graphics::Texture2D& grayTexture;
	const graphics::Texture2D& lightGrayTexture;
	const graphics::Texture2D& greenTexture;
	const graphics::Texture2D& redTexture;

	std::string buttonText;
	std::string textFieldText;
	std::string textDisplayText;
	std::string checkBoxText;
	std::string exit;

	Entity entity;
	Entity singleButton;
	Entity singleTextField;
	Entity singleTextDisplay;
	Entity singleCheckBox;

	std::vector<Entity> containerElements1;
	Entity container1;	

	std::vector<Entity> containerElements2;
	Entity container2;

	std::vector<Entity> containerElements3;
	Entity container3;
};
