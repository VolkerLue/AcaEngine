#include "FreeFall.hpp"
#include <engine\graphics\core\texture.cpp>
#include <engine\graphics\renderer\mesh.cpp>
#include <engine\graphics\renderer\meshrenderer.hpp>
#include <iostream>

FreeFall::FreeFall() : GameState() {
	difference = glm::translate(glm::vec3(0.f, 10.f, -20.0f));
}

void FreeFall::newState() {
	difference = glm::mat4(1.f);
}

void FreeFall::update(float _time, float _deltaTime) {
	float t = _time * _deltaTime;
}

void FreeFall::draw(float _time, float _deltaTime) {
	v = v + a * _deltaTime;
	difference = difference * glm::translate(glm::vec3(0.f, v, 0.f));
	if (difference[3][1] <= -10.f) {
		difference = glm::translate(glm::vec3(0.f, 10.f, -20.0f));
	}
	renderer->clear();
	renderer->draw(*mesh, *texture, difference);
	renderer->present(*camera);
}

void FreeFall::onResume() {
	float t = 0;
}

void FreeFall::onPause() {
	float t = 0;
}