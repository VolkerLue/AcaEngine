#include "shooter.hpp"


Shooter::Shooter() : GameState(), system(), shot(0), timePaused{ 0.0f } {
	std::vector<Entity> pointLights = EntityCreationInterface::createPointLights(system, 0.1f, 0.05f, 0.01f,
		std::vector<glm::vec3>(1, glm::vec3(0.f, 0.f, 0.f)), std::vector<glm::vec3>(1, glm::vec3(1.f, 1.f, 1.f)), std::vector<float>(1, 5.f));
	for (int i = 0; i < 50; i++) {
		entities.push_back(EntityCreationInterface::createRotatingCrate(system, glm::mat4(1.f), 
			glm::vec3(rand() % 51 + (-25), rand() % 51 + (-25), rand() % 10 + (-55)), glm::vec3(1.f, 0.5f, 1.f), glm::quat(1.0f, 0.f, 0.f, 0.f),
			glm::vec3(system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4)), 
			glm::vec3((((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5, (((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5, (((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5),
			false));
	}
}

void Shooter::newState() {
	finished = false;
	timePaused = 0.0f;
	for (auto it = entities.begin(); it != entities.end(); it++) {
		system.eraseEntity(*it);
	}
	entities.clear();
	for (int i = 0; i < 50; i++) {
		entities.push_back(EntityCreationInterface::createRotatingCrate(system, glm::mat4(1.f),
			glm::vec3(rand() % 51 + (-25), rand() % 51 + (-25), rand() % 10 + (-55)), glm::vec3(1.f, 0.5f, 1.f), glm::quat(1.0f, 0.f, 0.f, 0.f),
			glm::vec3(system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4)),
			glm::vec3((((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5, (((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5, (((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5),
			false));
	}
}

void Shooter::update(float _time, float _deltaTime) {
	_time += timePaused;
	if (_time > 100) finished = true;

	
	system.updatePosition(_deltaTime);
	system.updateOrientation(_deltaTime);
	system.updateTransform(_deltaTime);
	

	if (shot == 0 && input::InputManager::isButtonPressed(input::MouseButton::LEFT)) {
		glm::vec3 curserPos = system.cameraPerspective.toWorldSpace(input::InputManager::getCursorPos());
		entities.push_back(EntityCreationInterface::createShootablePlanet(system, glm::mat4(1.f), glm::vec3(curserPos), 
			glm::vec3(0.7f, 0.7f, 0.7f), glm::vec3(curserPos * 150.f), true));
		shot = 30;
	}
	else if (shot > 0) {
		shot--;
	}

	int deletedCrates = system.removeIntersecting();

	for (int i = 0; i < deletedCrates; i++) {
		entities.push_back(EntityCreationInterface::createRotatingCrate(system, glm::mat4(1.f),
			glm::vec3(rand() % 51 + (-25), rand() % 51 + (-25), rand() % 10 + (-55)), glm::vec3(1.f, 0.5f, 1.f), glm::quat(1.0f, 0.f, 0.f, 0.f),
			glm::vec3(system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4)),
			glm::vec3((((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5, (((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5, (((double)rand() / (RAND_MAX)) * 2 - 1) * 0.5),
			false));
	}

	int entityId = system.whichEntityIsNotInView();
	if (entityId != -1) {
		Entity entity{ static_cast<uint32_t>(entityId) };
		if (system.hasComponent<Orientation>(entity)) {
			system.setPosition(entity, glm::vec3(-75 + ((rand() % 2) * 150), -50 + ((rand() % 2) * 100), rand() % 10 + (-55)));
			system.setVelocity(entity, glm::vec3(system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4)));
		}
		else {
			system.eraseEntity(entity);
			entities.remove_if([&entity](const Entity& _entity) { return _entity.id == entity.id; });
		}
	}

}

void Shooter::draw(float _time, float _deltaTime) {
	system.draw();
}

void Shooter::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	system.draw();
}

void Shooter::onPause(float _time) {
	timePaused += _time;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Shooter::isFinished(){
	return false;
}

