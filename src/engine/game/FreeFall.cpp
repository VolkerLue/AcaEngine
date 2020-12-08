#pragma once
#include "FreeFall.hpp"
#include <GL/glew.h>

FreeFall::FreeFall() : GameState(), a(-0.01f), v(0.f),
	difference(glm::mat4(1.f)),
	camera(graphics::Camera(45.f, 0.1f, 10000.f)),
	texture(*graphics::Texture2DManager::get("textures/planet1.png",
	graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
	mesh(graphics::Mesh(*utils::MeshLoader::get("models/sphere.obj"))),
	renderer(graphics::MeshRenderer()) 
{
	utils::MeshLoader::clear();
	difference = glm::translate(glm::vec3(0.f, 10.f, -20.0f));
}

void FreeFall::newState() {
	difference = glm::translate(glm::vec3(0.f, 10.f, -20.0f));
}

void FreeFall::update(float _time, float _deltaTime) {
	if (_time > 15) {
		finished = true;
	}
}

void FreeFall::draw(float _time, float _deltaTime) {
	v = v + a * _deltaTime;
	difference = difference * glm::translate(glm::vec3(0.f, v, 0.f));
	if (difference[3][1] <= -10.f) {
		difference = glm::translate(glm::vec3(0.f, 10.f, -20.0f));
	}
	renderer.clear();
	renderer.draw(mesh, texture, difference);
	renderer.present(camera);
}

void FreeFall::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderer.clear();
	renderer.draw(mesh, texture, difference);
	renderer.present(camera);
}

void FreeFall::onPause() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
