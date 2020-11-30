#include <memory>
#include "GameState.hpp"
#include <GLFW\glfw3.h>
#include <memory>

class Game {
public:
	//initialize global resources
	Game();

	//finalize global resources
	~Game();

	//Start the game loop with _initialState on the stack
	void run(std::unique_ptr<GameState> _initialState);

	void addState(std::unique_ptr<GameState> _State);

	float maxDt;

private:
	GLFWwindow* window;
	std::vector<std::unique_ptr<GameState>> states;
};