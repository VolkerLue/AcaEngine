#include "UpAndDown.hpp"
#include <engine\graphics\core\texture.cpp>
#include <engine\graphics\renderer\mesh.cpp>
#include <engine\graphics\renderer\meshrenderer.hpp>
#include <iostream>



void UpAndDown::newState() {
	difference = glm::mat4(1.f);
}

void UpAndDown::update(float _time, float _deltaTime) {
	float t = _time * _deltaTime;
}

void UpAndDown::draw(float _time, float _deltaTime) {
	difference = difference * glm::translate(glm::vec3(0.f, 0.f, -1.1f));
	renderer->clear();
	renderer->draw(*mesh, *texture, difference);
	renderer->present(*camera);
}

void UpAndDown::onResume() {
	float t = 0;
}

void UpAndDown::onPause() {
	float t = 0;
}