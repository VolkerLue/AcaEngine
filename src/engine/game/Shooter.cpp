#include "FreeFall.hpp"
#include <GL/glew.h>

#include "Shooter.hpp"
#include <GL/glew.h>

Shooter::Shooter() : GameState(), system(),
texture(*graphics::Texture2DManager::get("textures/planet1.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
texture2(*graphics::Texture2DManager::get("textures/cratetex.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR)))
{
	for (int i = 0; i < 5; i++)
	{
		Entity entity;
		entities.push_back(system.createEntity(entity));
		system.addMesh(entities.back(), "models/sphere.obj");
		system.addTransform(entities.back(), glm::translate(glm::vec3(0.f, 10.f, -50.f)));
		system.addVelocity(entities.back(), glm::vec3(0.f, 0.13f, 0.f));
		system.addAccelaration(entities.back(), glm::vec3(0.f, -0.1f, 0.f));
		system.addMass(entities.back(), 1.f);
	}	
	for (int i = 0; i < 5; i++)
	{
		Entity entity;
		entities.push_back(system.createEntity(entity));
		system.addMesh(entities.back(), "models/crate.obj");
		system.addTransform(entities.back(), glm::translate(glm::vec3(0.f, 10.f, -50.f)));
		system.addVelocity(entities.back(), glm::vec3(0.f, 0.13f, 0.f));
		system.addAccelaration(entities.back(), glm::vec3(0.f, -0.1f, 0.f));
		system.addAnchor(entities.back(), glm::vec3(0.f, 0.f, 0.f));
	}
}

void Shooter::newState() {
}

void Shooter::update(float _time, float _deltaTime) {
	if (_time > 5) {
		finished = true;
	}

}

void Shooter::draw(float _time, float _deltaTime) {
	system.draw<Mesh, Transform, Mass>(texture);
	system.draw<Mesh, Transform, Anchor>(texture2);
}

void Shooter::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	system.draw<Mesh, Transform, Mass>(texture);
	system.draw<Mesh, Transform, Anchor>(texture2);
}

void Shooter::onPause() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Shooter::isFinished(){
	return false;
}