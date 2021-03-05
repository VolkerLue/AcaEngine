#include "buttondemo.hpp"
#include <iostream>

void displayMessage() {
	std::cout << "click" << "\n";
};

void writeText() {

}

ButtonDemo::ButtonDemo() :
	GameState(),
	system(),
	guiToolkit(system),
	darkBlueTexture(*graphics::Texture2DManager::get("textures/darkBlue.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
	lightBlueTexture(*graphics::Texture2DManager::get("textures/lightBlue.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
	whiteTexture(*graphics::Texture2DManager::get("textures/white.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
	lightGrayTexture(*graphics::Texture2DManager::get("textures/lightGray.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR)))
{
	finished = false;
	guiToolkit.addButton(glm::vec3(0.35f, 0.1f, 0.f), glm::vec3(0.3f, 0.1f, 1.f), lightBlueTexture, darkBlueTexture, displayMessage, (char*)"click me!");

	guiToolkit.addTextField(glm::vec3(0.f, 0.3f, 0.f), glm::vec3(0.3f, 0.1f, 1.f), lightGrayTexture, whiteTexture, displayMessage, (char*)"write here");
}

void ButtonDemo::newState() {
}

void ButtonDemo::update(float _time, float _deltaTime) {

	if (_time > 50) {
		finished = true;
	}

	system.updateTransform(_deltaTime);
	guiToolkit.update();
}

void ButtonDemo::draw(float _time, float _deltaTime) {
	system.draw();
}

void ButtonDemo::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ButtonDemo::onPause() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool ButtonDemo::isFinished() {
	return finished;
}