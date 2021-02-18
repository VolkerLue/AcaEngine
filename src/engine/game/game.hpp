#pragma once

#include "GameState.hpp"
#include "../graphics/core/device.hpp"
#include "../input/inputmanager.hpp"
#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>


class Game {
public:
	//initialize global resources
	Game();

	//finalize global resources
	~Game();

	//Start the game loop with _initialState on the stack
	void run(std::unique_ptr<GameState> _initialState);

	void addState(std::unique_ptr<GameState> _state);

private:
	GLFWwindow* window;
	std::vector<std::unique_ptr<GameState>> states;
};
