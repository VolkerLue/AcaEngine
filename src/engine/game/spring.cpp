#include "spring.hpp"


Spring::Spring() : GameState(), system(), 
texture(*graphics::Texture2DManager::get("textures/planet1.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
meshSphere(*utils::MeshLoader::get("models/sphere.obj"))
{	
	finished = false;
	entity = system.createEntity(entity);
	system.addMesh(entity, &meshSphere);
	system.addTexture(entity, &texture);
	system.addTransform(entity, glm::mat4(1.f));
	system.addPosition(entity, glm::vec3(0.f, 5.f, -15.0f));
	system.addVelocity(entity, glm::vec3(0.f, 0.f, 0.f));
	system.addAccelaration(entity, glm::vec3(0.f, 0.f, 0.f));
	system.addMass(entity, 1.f);
	system.addAnchor(entity, glm::vec3(0.f, 1.f, -15.f));
	system.addAABB(entity, false);

	system.addLightConstants(0.1, 0.05, 0.01);
	Entity entity1;
	system.addPointLight(system.createEntity(entity1), glm::vec3(0.f, 0.f, -10.f), glm::vec3(1.f, 1.f, 1.f), 5.f);
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
