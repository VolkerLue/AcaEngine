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
	const graphics::Texture2D& lightGrayTexture;
	std::string text1;
	std::string text2;

	Entity entity;
	std::list<Entity> entities;


};
