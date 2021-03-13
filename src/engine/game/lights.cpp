#include "lights.hpp"


Lights::Lights() : GameState(), system()

{
	meshEntity = EntityCreationInterface::createRotatingPlanet(system, glm::translate(glm::vec3(-15.f, 1.f, -20.0f)), glm::vec3(-15.f, 1.f, -20.f),
		glm::vec3(1.f, 1.f, 1.f), glm::quat(1.0f, 0.f, 0.f, 0.f), glm::vec3(3.f, 0.f, 0.f), glm::vec3(0.05f, 0.01f, 0.02f), true);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colors;
	std::vector<float> intensities;

	positions.push_back(glm::vec3(25.f, 1.f, -20.0f));
	colors.push_back(glm::vec3(10.f, 10.f, 10.f));
	intensities.push_back(1.8754f);

	positions.push_back(glm::vec3(0.f, 20.f, 0.f));
	colors.push_back(glm::vec3(10.f, 0.f, 0.f));
	intensities.push_back(2.f);
	
	positions.push_back(glm::vec3(-15.f, 1.f, -10.f));
	colors.push_back(glm::vec3(0.f, 3.f, 0.f));
	intensities.push_back(2.f);
	
	positions.push_back(glm::vec3(5.f, -10.f, -20.0f));
	colors.push_back(glm::vec3(0.f, 0.f, 7.f));
	intensities.push_back(2.f);

	lightEntities = EntityCreationInterface::createPointLights(system, 0.1f, 0.05f, 0.01f, positions, colors, intensities);

}

void Lights::newState() {
	system.eraseEntity(meshEntity);
	meshEntity = EntityCreationInterface::createRotatingPlanet(system, glm::translate(glm::vec3(-15.f, 1.f, -20.0f)), glm::vec3(-15.f, 1.f, -20.f),
		glm::vec3(1.f, 1.f, 1.f), glm::quat(1.0f, 0.f, 0.f, 0.f), glm::vec3(3.f, 0.f, 0.f), glm::vec3(0.05f, 0.01f, 0.02f), true);
}

void Lights::update(float _time, float _deltaTime) {
	_time += timePaused;
	if (_time > 12) finished = true;
	system.updateTransform(_deltaTime);
	system.updatePosition(_deltaTime);
	system.updateOrientation(_deltaTime);
}

void Lights::draw(float _time, float _deltaTime) {
	system.draw();
}

void Lights::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	system.draw();
}

void Lights::onPause(float _time) {
	timePaused += _time;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Lights::isFinished(){
	return false;
}
