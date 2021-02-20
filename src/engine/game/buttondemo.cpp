#include "buttondemo.hpp"

ButtonDemo::ButtonDemo() :
	GameState(),
	guiToolkit(),
	planeTexture(*graphics::Texture2DManager::get("textures/blue.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR)))
{
	guiToolkit.addButton(glm::vec3(-0.365f, -0.1f, -1.1f), glm::vec3(0.7f, 0.2f, 1.0f), planeTexture);
	text = "click me!";
	count = 0;
	pressed = false;
}

void ButtonDemo::newState() {
}

void ButtonDemo::update(float _time, float _deltaTime) {

	if (_time > 50) {
		finished = true;
	}

	if (input::InputManager::isButtonPressed(input::MouseButton::LEFT) == true &&
		input::InputManager::getCursorPos().x > 376 && 
		input::InputManager::getCursorPos().x < 963 && 
		input::InputManager::getCursorPos().y < 468 && 
		input::InputManager::getCursorPos().y > 300 &&
		pressed == false)
	{
		pressed = true;
		count++;
		text = std::to_string(count);
	}

	if (input::InputManager::isButtonPressed(input::MouseButton::LEFT) == false) {
		pressed = false;
	}
}

void ButtonDemo::draw(float _time, float _deltaTime) {
	guiToolkit.drawButtons(_deltaTime);
	guiToolkit.drawText(text, glm::vec3(0.f, 0.f, -1.0f), 0.1f, glm::vec4(1.f, 1.f, 1.f, 1.f), 0.f, 0.5f, 0.5f, false);
}

void ButtonDemo::onResume() {
}

void ButtonDemo::onPause() {
}

bool ButtonDemo::isFinished() {
	return false;
}