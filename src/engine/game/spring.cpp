#include "spring.hpp"


Spring::Spring() : GameState(), system() {	
	finished = false;
	std::vector<Entity> pointLights = EntityCreationInterface::createPointLights(system, 0.1f, 0.05f, 0.01f,
		std::vector<glm::vec3>(1, glm::vec3(0.f, 0.f, 0.f)), std::vector<glm::vec3>(1, glm::vec3(1.f, 1.f, 1.f)), std::vector<float>(1, 5.f));
	entity = EntityCreationInterface::createSpringPlanet(system, glm::mat4(1.f), glm::vec3(0.f, 5.f, -15.0f), glm::vec3(0.f, 0.f, 0.f), 
		glm::vec3(0.f, 0.f, 0.f), 1.f, glm::vec3(0.f, 1.f, -15.f));
}

void Spring::newState() {
	system.setPosition(entity, glm::vec3(0.f, 5.f, -15.0f));
	system.setVelocity(entity, glm::vec3(0.f, 0.f, 0.f));
	system.setAccelaration(entity, glm::vec3(0.f, 0.f, 0.f));
}

void Spring::update(float _time, float _deltaTime) {
	if (_time > 10) {
		finished = true;
	}
	system.springY(entity, _deltaTime);
	system.updateVelocity(_deltaTime);
	system.updatePosition(_deltaTime);
	system.updateTransform(_deltaTime);
}

void Spring::draw(float _time, float _deltaTime) {
	system.draw();
}

void Spring::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	system.draw();
}

void Spring::onPause() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Spring::isFinished() {
	return finished;
}
