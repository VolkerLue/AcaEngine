#include "game.hpp"


Game::Game() {
	//acquires global resources
	graphics::Device::initialize(1366, 768, false);
	window = graphics::Device::getWindow();
	input::InputManager::initialize(window);
	glClearColor(0.f, 1.f, 0.f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glfwSwapInterval(0); //VSync ON->(1) , OFF->(0)
	EntityCreationInterface::initialize();
}

Game::~Game() {
	graphics::Device::close(); 
	utils::MeshLoader::clear();
	graphics::ShaderManager::clear();
	graphics::Texture2DManager::clear();
}

void Game::addState(std::unique_ptr<GameState> _state) {
	states.push_back(std::move(_state));
}

void Game::run(std::unique_ptr<GameState> _initialState) {
	//manages game states with a stack invoking the appropriate events
	//controls delta time to maintain a smooth frame rate without wasting to much CPU time
	//performs state update + rendering

	using clock = std::chrono::high_resolution_clock;
	using duration_t = std::chrono::duration<float>;
	states.push_back(std::move(_initialState));
	bool rightPressed = false;
	bool leftPressed = false;

	while (!states.empty() && !glfwWindowShouldClose(window)) {
		//states saved number or second stack
		//interuption keys
		//saving of t in state
		GameState& current = *states.back();
		current.onResume();
		float t = 0;
		const float timeStep = 0.01;
		auto currentTime = clock::now();
		float dt = 0.f;
		rightPressed = false;
		leftPressed = false;
		while (!current.isFinished() && !glfwWindowShouldClose(window) && !leftPressed && !rightPressed) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			auto newTime = clock::now();
			duration_t frameTime = newTime - currentTime;
			currentTime = newTime;
			dt += frameTime.count();
			while (dt >= timeStep) {
				t += timeStep;
				dt -= timeStep;
				current.update(t, timeStep);
			}
			current.draw(t, dt / timeStep);

			if (input::InputManager::isKeyPressed(input::Key::RIGHT)) {
				rightPressed = true;
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
			if (input::InputManager::isKeyPressed(input::Key::LEFT)) {
				leftPressed = true;
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
			glfwPollEvents();
			glfwSwapBuffers(window);
		}

		if (rightPressed) {
			current.onPause(t);
			std::move(states.end() - 1, states.end(), std::back_inserter(pausedStates));
			states.pop_back();
		}
		else if (leftPressed) {
			if (pausedStates.size() == 0) {
				current.newState();
			}
			else {
				current.onPause(t);
				std::move(pausedStates.end() - 1, pausedStates.end(), std::back_inserter(states));
				pausedStates.pop_back();
			}
		}
		else {
			current.newState();
			std::move(states.end() - 1, states.end(), std::back_inserter(pausedStates));
			states.pop_back();
		}

	}
}
