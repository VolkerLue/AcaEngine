#pragma once

#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <engine/gui_toolkit/entity_creation_interface.hpp>

#include "gamestate.hpp"
#include "../graphics/core/device.hpp"
#include "../input/inputmanager.hpp"


class Game {
public:
	//initialize global resources
	Game();

	//finalize global resources
	~Game();

	//Start the game loop with _initialState on the stack
	//Keys Left, Right and Numbers to switch between states, Space to restart State
	void run(std::unique_ptr<GameState> _initialState);

	void chooseState(float time, int number);

	void addState(std::unique_ptr<GameState> _state);
	
private:
	GLFWwindow* window;	
	std::vector<std::unique_ptr<GameState>> states;
	std::vector<std::unique_ptr<GameState>> pausedStates;
};
