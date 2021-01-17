#include "FreeFall.hpp"
#include <GL/glew.h>

FreeFall::FreeFall() : GameState(), system(),
texture(*graphics::Texture2DManager::get("textures/planet1.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
texture2(*graphics::Texture2DManager::get("textures/cratetex.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR)))
{
	entity = system.createEntity(entity);
	system.addMesh(entity, "models/sphere.obj");
	system.addTransform(entity, glm::translate(glm::vec3(0.f, 10.f, -50.f)));
	system.addVelocity(entity, glm::vec3(0.f, 0.13f, 0.f));
	system.addAccelaration(entity, glm::vec3(0.f, -0.1f, 0.f));

	entity2 = system.createEntity(entity2);
	system.addMesh(entity2, "models/crate.obj");
	system.addTransform(entity2, glm::translate(glm::vec3(5.f, 10.f, -50.f)));
	system.addVelocity(entity2, glm::vec3(0.f, 0.13f, 0.f));
	system.addAccelaration(entity2, glm::vec3(0.f, -0.1f, 0.f));
}

void FreeFall::newState() {
}

void FreeFall::update(float _time, float _deltaTime) {
	if (_time > 5) {
		finished = true;
	}
	system.move(entity, _deltaTime);
	system.move(entity2, _deltaTime);
}

void FreeFall::draw(float _time, float _deltaTime) {
	system.drawEntity(entity, texture);
	system.drawEntity(entity2, texture2);
}

void FreeFall::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	system.drawEntity(entity, texture);
}

void FreeFall::onPause() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool FreeFall::isFinished(){
	return false;
}
