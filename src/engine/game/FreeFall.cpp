#include "FreeFall.hpp"
#include <GL/glew.h>

FreeFall::FreeFall() : GameState(), system(), texture(*graphics::Texture2DManager::get("textures/planet1.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR)))
{
	entity = system.createEntity(entity);
	system.addMesh(entity, "models/sphere.obj");
	system.addTransform(entity, glm::translate(glm::vec3(0.f, 10.f, -50.f)));
	system.addVelocity(entity, glm::vec3(0.f, 0.13f, 0.f));
	system.addAccelaration(entity, glm::vec3(0.f, -0.1f, 0.f));
}

void FreeFall::newState() {
}

void FreeFall::update(float _time, float _deltaTime) {
	if (_time > 5) {
		finished = true;
	}
	system.move(entity, _deltaTime);
}

void FreeFall::draw(float _time, float _deltaTime) {
	system.draw(entity, texture);
}

void FreeFall::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	system.draw(entity, texture);
}

void FreeFall::onPause() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool FreeFall::isFinished(){
	return false;
}
