#include "upanddown.hpp"


UpAndDown::UpAndDown() : GameState(), system(), 
texture(*graphics::Texture2DManager::get("textures/planet1.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
meshSphere(*utils::MeshLoader::get("models/sphere.obj"))
{
	entity = system.createEntity(entity);
	system.addMesh(entity, &meshSphere);
	system.addTransform(entity, glm::translate(glm::vec3(0.f, 4.f, -5.0f)));
	system.addVelocity(entity, glm::vec3(0.f, 0.f, 0.f));
	system.addAccelaration(entity, glm::vec3(0.f, 0.f, 0.f));
}

void UpAndDown::newState() {
}

void UpAndDown::update(float _time, float _deltaTime) {
	if (_time > 5) {
		finished = true;
	}
	if ((int)_time % 2 == 0) {
		system.setVelocity(entity, glm::vec3(0.f, -0.05f, 0.f));
		system.move(entity, _deltaTime);
	}
	else {
		system.setVelocity(entity, glm::vec3(0.f, +0.05f, 0.f));
		system.move(entity, _deltaTime);
	}
}

void UpAndDown::draw(float _time, float _deltaTime) {
	system.drawEntity(entity, texture);
}

void UpAndDown::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	system.drawEntity(entity, texture);
}

void UpAndDown::onPause() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool UpAndDown::isFinished() {
	return false;
}
