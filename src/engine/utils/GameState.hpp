#include <engine/graphics/renderer/meshrenderer.hpp>

class GameState {
public:
	GameState() : finished(false) {
		using namespace graphics;
		
		Sampler sampler(Sampler::Filter::LINEAR, Sampler::Filter::LINEAR, Sampler::Filter::LINEAR);
		const Texture2D& texturePlanet = *Texture2DManager::get("textures/planet1.png", sampler);
		texture = &texturePlanet;
		
		Mesh meshPlanet(*utils::MeshLoader::get("models/sphere.obj"));
		mesh = &meshPlanet;
		
		MeshRenderer renderer;
		this->renderer = &renderer;
		
		Camera camera(45.f, 0.1f, 10000.f);
		this->camera = &camera;
		
		difference = glm::mat4(1.f);
		finished = false;
	}



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

	bool finished;
	const graphics::Texture2D* texture;
	graphics::Mesh* mesh;
	graphics::Camera* camera;
	graphics::MeshRenderer* renderer;
	glm::mat4 difference;
};
