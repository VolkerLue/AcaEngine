#include "Shooter.hpp"


Shooter::Shooter() : GameState(), system(),
texturePlanet(*graphics::Texture2DManager::get("textures/planet1.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
textureCratetex(*graphics::Texture2DManager::get("textures/cratetex.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
meshSphere(*utils::MeshLoader::get("models/sphere.obj")),
meshCrate(*utils::MeshLoader::get("models/crate.obj")),
shot(0)
{
	srand(time(NULL));
	for (int i = 0; i < 100; i++){
		Entity entity;
		entities.push_back(system.createEntity(entity));				

		system.addTexture(entities.back(), &textureCratetex);
		system.addMesh(entities.back(), &meshCrate);
		system.addTransform(entities.back(), glm::translate(glm::vec3(0.f, 0.f, float(rand() % 10 + (-55)))));
		system.addVelocity(entities.back(), glm::vec3(system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4)));
		system.addRotation(entities.back(), glm::radians(float(rand() % 20 + (10))), glm::vec3(rand() % 2, rand() % 2, rand() % 2));
		system.addAABB(entities.back(), false);
	}
}

void Shooter::newState() {
}

void Shooter::update(float _time, float _deltaTime) {
	if (_time > 100) finished = true;

	system.updateTransform(_deltaTime);
	system.removeEntityWhenNotInView(entities);

	if (shot == 0 && input::InputManager::isButtonPressed(input::MouseButton::LEFT)) {
		system.shootMeshWithTexure(&meshSphere, texturePlanet, entities, 500.f);
		shot = 30;
	}
	else if (shot > 0) {
		shot--;
	}
	system.updateAABB();
	int deletedCrates = system.removeIntersecting();

	srand(time(NULL));
	for (int i = 0; i < deletedCrates; i++) {
		Entity entity;
		entities.push_back(system.createEntity(entity));
		system.addTexture(entities.back(), &textureCratetex);
		system.addMesh(entities.back(), &meshCrate);
		system.addTransform(entities.back(), glm::translate(glm::vec3(0.f, 0.f, float(rand() % 10 + (-55)))));
		system.addVelocity(entities.back(), glm::vec3(system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4)));
		system.addRotation(entities.back(), glm::radians(float(rand() % 20 + (10))), glm::vec3(rand() % 2, rand() % 2, rand() % 2));
		system.addAABB(entities.back(), false);
	}
}

void Shooter::draw(float _time, float _deltaTime) {
	system.draw();
}

void Shooter::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	system.draw();
}

void Shooter::onPause() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Shooter::isFinished(){
	return false;
}

