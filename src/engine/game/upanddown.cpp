#include "upanddown.hpp"


UpAndDown::UpAndDown() : GameState(), system(), timePaused{ 0.0f } {
	std::vector<Entity> pointLights = EntityCreationInterface::createPointLights(system, 0.1f, 0.05f, 0.01f,
		std::vector<glm::vec3>(1, glm::vec3(0.f, 0.f, 0.f)), std::vector<glm::vec3>(1, glm::vec3(1.f, 1.f, 1.f)), std::vector<float>(1, 5.f));
	entity = EntityCreationInterface::createMovingPlanet(system, glm::translate(glm::vec3(0.f, 4.f, -12.0f)), glm::vec3(0.f, 0.f, 0.f), 
		glm::vec3(0.f, 0.f, 0.f));
}

void UpAndDown::newState() {
	finished = false;
	timePaused = 0.0f;
	system.eraseEntity(entity);
	entity = EntityCreationInterface::createMovingPlanet(system, glm::translate(glm::vec3(0.f, 4.f, -12.0f)), glm::vec3(0.f, 0.f, 0.f), 
		glm::vec3(0.f, 0.f, 0.f));
}

void UpAndDown::update(float _time, float _deltaTime) {
	_time += timePaused;
	if (_time > 60) {
		finished = true;
	}
	if ((int)_time % 2 == 0) {
		system.setVelocity(entity, glm::vec3(0.f, -0.1f, 0.f));
		system.move(entity, _deltaTime);
	}
	else {
		system.setVelocity(entity, glm::vec3(0.f, +0.1f, 0.f));
		system.move(entity, _deltaTime);
	}
}

void UpAndDown::draw(float _time, float _deltaTime) {
	system.draw();
}

void UpAndDown::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	system.draw();
}

void UpAndDown::onPause(float _time) {
	timePaused += _time;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool UpAndDown::isFinished() {
	return false;
}
