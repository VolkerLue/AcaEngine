#include "Spring.hpp"


Spring::Spring() : GameState(), system(), 
texture(*graphics::Texture2DManager::get("textures/planet1.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR)))
{
	entity = system.createEntity(entity);
	system.addMesh(entity, "models/sphere.obj");
	system.addTransform(entity, glm::translate(glm::vec3(0.f, 4.f, -10.0f)));
	system.addVelocity(entity, glm::vec3(0.f, 0.f, 0.f));
	system.addAccelaration(entity, glm::vec3(0.f, -10.f, 0.f));
	system.addMass(entity, 10.f);
	system.addAnchor(entity, glm::vec3(0.f, 0.f, -10.f));
}

void Spring::newState() {
}

void Spring::update(float _time, float _deltaTime) {
	if (_time > 10) {
		finished = true;
	}
	system.springY(entity, _deltaTime);
}

void Spring::draw(float _time, float _deltaTime) {
	system.drawEntity(entity, texture);
}

void Spring::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	system.drawEntity(entity, texture);
}

void Spring::onPause() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Spring::isFinished() {
	return false;
}
