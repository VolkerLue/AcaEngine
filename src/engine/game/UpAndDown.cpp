#pragma once
#include "UpAndDown.hpp"
#include <GL/glew.h>

UpAndDown::UpAndDown() : GameState(),
	difference(glm::mat4(1.f)),
	camera(graphics::Camera(45.f, 0.1f, 10000.f)),
	texture(*graphics::Texture2DManager::get("textures/planet1.png",
	graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
	mesh(graphics::Mesh(*utils::MeshLoader::get("models/sphere.obj"))),
	renderer(graphics::MeshRenderer())
{
	utils::MeshLoader::clear();
	difference = glm::translate(glm::vec3(0.f, 4.f, -5.0f));
}

void UpAndDown::newState() {
	difference = glm::translate(glm::vec3(0.f, 4.f, -5.0f));
}

void UpAndDown::update(float _time, float _deltaTime) {
	if (_time > 5) {
		finished = true;
	}
	if ((int)_time % 2 == 0) {
		difference *= glm::translate(glm::vec3(0.f, -0.05f, 0.f));
	}
	else {
		difference *= glm::translate(glm::vec3(0.f, +0.05f, 0.f));
	}
}

void UpAndDown::draw(float _time, float _deltaTime) {
	renderer.clear();
	renderer.draw(mesh, texture, difference);
	renderer.present(camera);
}

void UpAndDown::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderer.clear();
	renderer.draw(mesh, texture, difference);
	renderer.present(camera);
}

void UpAndDown::onPause() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
