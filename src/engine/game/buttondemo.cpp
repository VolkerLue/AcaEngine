#include "buttondemo.hpp"
#include <iostream>

void displayClick(Entity& _entity, System& _system) {
	std::cout << "click" << "\n";
};

void displayText(Entity& _entity, System& _system) {
	std::string text = _system.registry.getComponentUnsafe<Text>(_entity).text;
	std::cout << text << "\n";
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
	
	entities.push_back(system.createEntity(entity));
	text1 = "click me!";
	guiToolkit.addButton(entities.back(), glm::vec3(0.35f, 0.1f, 0.f), glm::vec3(0.3f, 0.1f, 1.f), lightBlueTexture, darkBlueTexture, displayClick, text1);

	entities.push_back(system.createEntity(entity));
	guiToolkit.addButton(entities.back(), glm::vec3(0.35f, 0.3f, 0.f), glm::vec3(0.3f, 0.1f, 1.f), lightBlueTexture, darkBlueTexture, displayClick, text1);
		
	entities.push_back(system.createEntity(entity));
	text2 = "write here!";
	guiToolkit.addTextField(entities.back(), glm::vec3(0.35f, 0.5f, 0.f), glm::vec3(0.3f, 0.1f, 1.f), lightGrayTexture, whiteTexture, displayText, text2);

	entities.push_back(system.createEntity(entity));
	guiToolkit.addTextField(entities.back(), glm::vec3(0.35f, 0.7f, 0.f), glm::vec3(0.3f, 0.1f, 1.f), lightGrayTexture, whiteTexture, displayText, text2);
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

void ButtonDemo::onPause(float _time) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool ButtonDemo::isFinished() {
	return finished;
}