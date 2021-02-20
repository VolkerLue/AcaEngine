#pragma once
#include "gamestate.hpp"
#include "../gui_toolkit/guitoolkit.hpp"

class ButtonDemo : public GameState {
public:
	ButtonDemo();
	void update(float _time, float _deltaTime) override;
	void draw(float _time, float _deltaTime) override;

	void onResume() override;
	void onPause() override;

	void newState() override;
	bool isFinished();

private:
	GuiToolkit guiToolkit;
	const graphics::Texture2D& planeTexture;	
	std::string text;
	int count;
	bool pressed;
};
