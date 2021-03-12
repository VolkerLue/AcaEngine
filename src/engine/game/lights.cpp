#include "lights.hpp"


lights::lights() : GameState(), system(),
texturePlanet(*graphics::Texture2DManager::get("textures/planet1.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
textureCratetex(*graphics::Texture2DManager::get("textures/cratetex.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
meshSphere(*utils::MeshLoader::get("models/sphere.obj")),
meshCrate(*utils::MeshLoader::get("models/crate.obj"))

{
	

	Entity entity;
	entities.push_back(system.createEntity(entity));

	system.addTexture(entities.back(), &texturePlanet);
	system.addMesh(entities.back(), &meshSphere);
	system.addTransform(entities.back(), glm::translate(glm::vec3(-15.f, 1.f, -20.0f)));

	system.addPosition(entities.back(), glm::vec3(-15.f, 1.f, -20.f));
	system.addOrientation(entities.back(), glm::quat(1.0f, 0.f, 0.f, 0.f));
	system.addScale(entities.back(), glm::vec3(1.f, 1.f, 1.f));

	system.addVelocity(entities.back(), glm::vec3(3.f, 0.f, 0.f));
	system.addAngularVelocity(entities.back(), glm::vec3(0.05f, 0.01f, 0.02f));

	system.addAABB(entities.back(), false);


	system.addLightConstants(0.1, 0.05, 0.01);
	
	Entity entity2;
	system.addPointLight(system.createEntity(entity2), glm::vec3(25.f, 1.f, -20.0f), glm::vec3(10.f, 10.f, 10.f), 1.8754f);
	Entity entity3;
	system.addPointLight(system.createEntity(entity3), glm::vec3(0.f, 20.f, 0.f), glm::vec3(10.f, 0.f, 0.f), 2.f);
	Entity entity4;
	system.addPointLight(system.createEntity(entity4), glm::vec3(-15.f, 1.f, -10.f), glm::vec3(0.f, 3.f, 0.f), 2.f);
	Entity entity5;
	system.addPointLight(system.createEntity(entity5), glm::vec3(5.f, -10.f, -20.0f), glm::vec3(0.f, 0.f, 7.f), 2.f);
	

}

void lights::newState() {
}

void lights::update(float _time, float _deltaTime) {
	if (_time > 12) finished = true;

	system.updateTransform(_deltaTime);
	system.updatePosition(_deltaTime);
	system.updateOrientation(_deltaTime);

	

}

void lights::draw(float _time, float _deltaTime) {
	system.draw();
}

void lights::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	system.draw();
}

void lights::onPause() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool lights::isFinished(){
	return false;
}
