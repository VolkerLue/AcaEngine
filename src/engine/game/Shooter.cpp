#include "Shooter.hpp"


Shooter::Shooter() : GameState(), system(),
texturePlanet(*graphics::Texture2DManager::get("textures/planet1.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
textureCratetex(*graphics::Texture2DManager::get("textures/cratetex.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR)))
{
	srand(time(NULL));
	for (int i = 0; i < 100; i++)
	{
		Entity entity;
		entities.push_back(system.createEntity(entity));
				
		system.addMesh(entities.back(), "models/crate.obj");
		system.addTransform(entities.back(), glm::translate(glm::vec3(0.f, 0.f, float(rand() % 10 + (-55)))));
		system.addVelocity(entities.back(), glm::vec3(system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4), system.randomWithoutZero(9, -4)));
		system.addAlive(entities.back(), true);
		system.addRotation(entities.back(), glm::radians(float(rand() % 20 + (10))), glm::vec3(rand() % 2, rand() % 2, rand() % 2));
	}
	for (int i = 0; i < 100; i++)
	{
		Entity entity;
		entities.push_back(system.createEntity(entity));

		system.addMesh(entities.back(), "models/sphere.obj");
		system.addTransform(entities.back(), glm::translate(glm::vec3(0.f, 0.f, 0.f)));
		system.addVelocity(entities.back(), glm::vec3(0.f, 0.f, 0.f));
		system.addAlive(entities.back(), false);
		system.addCursorPosition(entities.back(), glm::vec3(0.f, 0.f, 0.f));
	}
}

void Shooter::newState() {
}

void Shooter::update(float _time, float _deltaTime) {
	if (_time > 100) {
		finished = true;
	}	
	system.updateTransformCrate(_deltaTime);
	system.repositionCrate();	
	system.updateTransformPlanet(_deltaTime);
	system.updateShoot(entities);
}

void Shooter::draw(float _time, float _deltaTime) {
	system.draw<Mesh, Transform, Alive, Rotation>(textureCratetex);
	system.draw<Mesh, Transform, Alive, CursorPosition>(texturePlanet);
}

void Shooter::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	system.draw<Mesh, Transform, Alive, Rotation>(textureCratetex);
	system.draw<Mesh, Transform, Alive, CursorPosition>(texturePlanet);
}

void Shooter::onPause() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Shooter::isFinished(){
	return false;
}
