#pragma once
#include <engine/utils/Spring.hpp>
#include <GL/glew.h>

Spring::Spring() : GameState(), times(0) {}

void Spring::newState() {
	difference = glm::mat4(1.f);
}

void Spring::update(float _time, float _deltaTime) {
	times = times + 1;
	if (_time > 5) {
		finished = true;
	}

}

void Spring::draw(float _time, float _deltaTime) {
	float velo = 300 * sin((_time) * 3.14 / 180) * _deltaTime;
	if (times < 50) {
		difference = difference * glm::translate(glm::vec3(0.f, 0.f, -1.1f));
	}
	else {

		difference = difference * glm::translate(glm::vec3(velo, 0.f, 0.f));
	}


	renderer->clear();
	renderer->draw(*mesh, *texture, difference);
	renderer->present(*camera);
}

void Spring::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderer->clear();
	renderer->draw(*mesh, *texture, difference);
	renderer->present(*camera);
}

void Spring::onPause() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//auto test_start = std::chrono::steady_clock::now();
//float test_time = std::chrono::duration_cast<TimeT>(std::chrono::steady_clock::now() - test_start).count();