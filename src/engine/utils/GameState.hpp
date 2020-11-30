#include <engine\graphics\renderer\meshrenderer.hpp>

class GameState {
public:

	//has pure virtual functions for frame updates
	virtual void update(float _time, float _deltaTime) = 0;
	virtual void draw(float _time, float _deltaTime) = 0;

	//has virtual interface for state transition events
	virtual void onResume() = 0;
	virtual void onPause() = 0;

	//interface to mark current state as finished and to provide a new state
	virtual void newState() = 0;
	bool isFinished() {
		return finished;
	}

private:
	bool finished;
	graphics::MeshRenderer renderer;
};