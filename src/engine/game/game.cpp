#include "game.hpp"
#include "gamehelper.hpp"


int pressedNumber;
bool numPressed;

Game::Game() {
	//acquires global resources
	graphics::Device::initialize(3000, 2000, false); // fullScreen -> false or true -> has no effect -> always no fullScreen -> solution -> glfwGetPrimaryMonitor()
	window = graphics::Device::getWindow();
	input::InputManager::initialize(window);
	glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glfwSwapInterval(0); //VSync ON->(1) , OFF->(0)
	srand(static_cast<unsigned int>(time(NULL)));
	EntityCreationInterface::initialize();
}

Game::~Game() {
	graphics::Device::close(); 
	utils::MeshLoader::clear();
	graphics::ShaderManager::clear();
	graphics::Texture2DManager::clear();
}

void Game::addState(std::shared_ptr<GameState> _state) {
	states.push_back(std::move(_state));
}

void Game::run(std::shared_ptr<GameState> _initialState) {
	//manages game states with a stack invoking the appropriate events
	//controls delta time to maintain a smooth frame rate without wasting to much CPU time
	//performs state update + rendering

	using clock = std::chrono::high_resolution_clock;
	using duration_t = std::chrono::duration<float>;
	states.push_back(std::move(_initialState));
	bool rightPressed = false, leftPressed = false, spacePressed = false, /*numPressed = false,*/ num1Pressed = false, num2Pressed = false, num3Pressed = false, num4Pressed = false,
		num5Pressed = false, num6Pressed = false, num7Pressed = false, num8Pressed = false, num9Pressed = false;
	

	while (!states.empty() && !glfwWindowShouldClose(window)) {
		//states saved number or second stack
		//interuption keys
		//saving of t in state
		std::shared_ptr<GameState> current = states.back();
		
		current->onResume();
		float t = 0;
		const float timeStep = 0.01;
		auto currentTime = clock::now();
		float dt = 0.f;
		rightPressed = leftPressed = spacePressed = numPressed = num1Pressed = num2Pressed = num3Pressed = num4Pressed = num5Pressed = num6Pressed = num7Pressed = 
			num8Pressed = num9Pressed = false;

		pressedNumber = 0;

		while (!current->isFinished() && !glfwWindowShouldClose(window) && !leftPressed && !rightPressed && !spacePressed && !numPressed) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			auto newTime = clock::now();
			duration_t frameTime = newTime - currentTime;
			currentTime = newTime;
			dt += frameTime.count();
			while (dt >= timeStep) {
				t += timeStep;
				dt -= timeStep;
				current->update(t, timeStep);
			}
			current->draw(t, dt / timeStep);
			glfwPollEvents();
			glfwSwapBuffers(window);

			if (input::InputManager::isKeyPressed(input::Key::RIGHT)) {
				rightPressed = true;
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
			else if (input::InputManager::isKeyPressed(input::Key::LEFT)) {
				leftPressed = true;
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
			else if (input::InputManager::isKeyPressed(input::Key::SPACE)) {
				spacePressed = true;
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
			else if (input::InputManager::isKeyPressed(input::Key::Num1)) {
				numPressed = true;
				num1Pressed = true;
				pressedNumber = 1;
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
			else if (input::InputManager::isKeyPressed(input::Key::Num2)) {
				numPressed = true;
				num2Pressed = true;
				pressedNumber = 2;
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
			else if (input::InputManager::isKeyPressed(input::Key::Num3)) {
				numPressed = true;
				num3Pressed = true;
				pressedNumber = 3;
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
			else if (input::InputManager::isKeyPressed(input::Key::Num4)) {
				numPressed = true;
				num4Pressed = true;
				pressedNumber = 4;
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
			else if (input::InputManager::isKeyPressed(input::Key::Num5)) {
				numPressed = true;
				num5Pressed = true;
				pressedNumber = 5;
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
			else if (input::InputManager::isKeyPressed(input::Key::Num6)) {
				numPressed = true;
				num6Pressed = true;
				pressedNumber = 6;
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
			else if (input::InputManager::isKeyPressed(input::Key::Num7)) {
				numPressed = true;
				num7Pressed = true;
				pressedNumber = 7;
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
			else if (input::InputManager::isKeyPressed(input::Key::Num8)) {
				numPressed = true;
				num8Pressed = true;
				pressedNumber = 8;
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
			else if (input::InputManager::isKeyPressed(input::Key::Num9)) {
				numPressed = true;
				num9Pressed = true;
				pressedNumber = 9;
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
		}

		if (rightPressed) {
			current->onPause(t);
			std::move(states.end() - 1, states.end(), std::back_inserter(pausedStates));
			states.pop_back();
		}
		else if (leftPressed) {
			if (pausedStates.size() == 0) {
				current->newState();
			}
			else {
				current->onPause(t);
				std::move(pausedStates.end() - 1, pausedStates.end(), std::back_inserter(states));
				pausedStates.pop_back();
			}
		}
		else if (spacePressed) {
			current->newState();
		}
		else if (numPressed) {
			chooseState(t, pressedNumber);
		}
		else {
			current->newState();
			std::move(states.end() - 1, states.end(), std::back_inserter(pausedStates));
			states.pop_back();
		}
		if (glfwWindowShouldClose(window) || states.empty()) {
			for (auto it = states.begin(); it != states.end(); it++) {
				it->reset();
			}
			for (auto it = pausedStates.begin(); it != pausedStates.end(); it++) {
				it->reset();
			}
		}
	}
}

void Game::chooseState(float time,  int number) {
	states.back()->onPause(time);
	int numPausedStates = pausedStates.size();
	int numRemainingStates = states.size();
	int totalStates = numPausedStates + numRemainingStates;
	if (number <= numPausedStates) {		//backwards
		int difference = numPausedStates - number + 1;
		for (int i = 0; i < difference; i++) {
			std::move(pausedStates.end() - 1, pausedStates.end(), std::back_inserter(states));
			pausedStates.pop_back();
		}
	}
	else if (number == numPausedStates + 1) {	//current
		return;
	}
	else if (number <= totalStates) {		//forewards
		int difference = number - numPausedStates - 1;
		for (int i = 0; i < difference; i++) {
			std::move(states.end() - 1, states.end(), std::back_inserter(pausedStates));
			states.pop_back();
		}
	}
	else {
		states.clear();	//correct memory management?
	}

}
