#include "freefall.hpp"


FreeFall::FreeFall() : GameState(), system(), timePaused{ 0.0f } {
	std::vector<Entity> pointLights = EntityCreationInterface::createPointLights(system, 0.1f, 0.05f, 0.01f,
		std::vector<glm::vec3>(1, glm::vec3(0.f, 0.f, 0.f)), std::vector<glm::vec3>(1, glm::vec3(1.f, 1.f, 1.f)), std::vector<float>(1, 10.f));
	entity = EntityCreationInterface::createMovingPlanet(system, glm::translate(glm::vec3(0.f, 10.f, -30.f)), glm::vec3(0.f, 0.13f, 0.f),
		glm::vec3(0.f, -0.1f, 0.f));
	entity2 = EntityCreationInterface::createMovingCrate(system, glm::translate(glm::vec3(5.f, 10.f, -30.f)), glm::vec3(0.f, 0.13f, 0.f),
		glm::vec3(0.f, -0.1f, 0.f));
}

void FreeFall::newState() {
	finished = false;
	timePaused = 0.0f;
	system.eraseEntity(entity);
	system.eraseEntity(entity2);
	entity = EntityCreationInterface::createMovingPlanet(system, glm::translate(glm::vec3(0.f, 10.f, -30.f)), glm::vec3(0.f, 0.13f, 0.f),
		glm::vec3(0.f, -0.1f, 0.f));
	entity2 = EntityCreationInterface::createMovingCrate(system, glm::translate(glm::vec3(5.f, 10.f, -30.f)), glm::vec3(0.f, 0.13f, 0.f),
		glm::vec3(0.f, -0.1f, 0.f));
}

void FreeFall::update(float _time, float _deltaTime) {
	_time += timePaused;
	if (_time > 5) {
		finished = true;
	}
	system.move(entity, _deltaTime);
	system.move(entity2, _deltaTime);
}

void FreeFall::draw(float _time, float _deltaTime) {
	system.draw();
}

void FreeFall::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	system.draw();
}

void FreeFall::onPause(float _time) {
	timePaused += _time;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool FreeFall::isFinished(){
	return false;
}
