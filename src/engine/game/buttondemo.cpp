#include "buttondemo.hpp"
#include <iostream>


void functionExampleButton0(Entity& _entity, System& _system) {
	std::cout << "click" << "\n";
	glClearColor(0.f, 0.f, 0.f, 0.f);
};

void functionExampleButton1(Entity& _entity, System& _system) {
	std::cout << "click" << "\n";
	glClearColor(1.f, 1.f, 1.f, 1.f);
};

static bool buttonDemoFinished = false;
void functionExampleButton2(Entity& _entity, System& _system) {
	buttonDemoFinished = true;	
};

void functionExampleTextField(Entity& _entity, System& _system) {
	std::string text = _system.registry.getComponentUnsafe<Text>(_entity).text;
	std::cout << text << "\n";
}

void functionExampleCheckBox(Entity& _entity, System& _system) {
	if (_system.registry.getComponentUnsafe<CheckBox>(_entity).status == true) {
		glClearColor(0.f, 0.f, 0.f, 0.f);
	}
	else {
		glClearColor(1.f, 1.f, 1.f, 1.f);
	}
}

ButtonDemo::ButtonDemo() :
	GameState(),
	system(),
	guiToolkit(system),
	darkBlueTexture(*graphics::Texture2DManager::get("textures/darkBlue.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
	lightBlueTexture(*graphics::Texture2DManager::get("textures/lightBlue.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
	whiteTexture(*graphics::Texture2DManager::get("textures/white.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
	grayTexture(*graphics::Texture2DManager::get("textures/gray.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
	lightGrayTexture(*graphics::Texture2DManager::get("textures/lightGray.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
	greenTexture(*graphics::Texture2DManager::get("textures/green.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
	redTexture(*graphics::Texture2DManager::get("textures/red.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR)))
{
	finished = false;

	buttonText = "Button";
	textFieldText = "TextField";
	textDisplayText = "TextDisplay";
	checkBoxText = "CheckBox";
	exit = "EXIT";

	// single button with all arguments
	singleButton = system.createEntity(singleButton);
	guiToolkit.addButton(singleButton, glm::vec3(0.01f, 0.01f, 0.f), glm::vec3(0.3f, 0.05f, 1.f), lightBlueTexture, darkBlueTexture, true, functionExampleButton2, exit, glm::vec4(1.f));

	// single textField with all arguments
	singleTextField = system.createEntity(singleTextField);
	guiToolkit.addTextField(singleTextField, glm::vec3(0.35f, 0.01f, 0.f), glm::vec3(0.3f, 0.05f, 1.f), grayTexture, whiteTexture, functionExampleTextField, textFieldText, glm::vec4(0.f));

	// single textDisplay with all arguments
	singleTextDisplay = system.createEntity(singleTextDisplay);
	guiToolkit.addTextDisplay(singleTextDisplay, glm::vec3(0.69f, 0.01f, 0.f), glm::vec3(0.3f, 0.05f, 1.f), whiteTexture, textDisplayText, glm::vec4(0.f));

	// single checkBox with all arguments
	singleCheckBox = system.createEntity(singleCheckBox);
	guiToolkit.addCheckBox(singleCheckBox, glm::vec3(0.01f, 0.94f, 0.f), glm::vec3(0.2f, 0.05f, 1.f), redTexture, greenTexture, functionExampleCheckBox, checkBoxText, glm::vec4(0.f));

	// single Slider
	Entity sliderEntity;
	system.createEntity(sliderEntity);
	guiToolkit.addSlider(sliderEntity, glm::vec3(0.22f, 0.94f, 0.f), glm::vec3(0.2f, 0.05f, 1.f), 10, 0, true);

	/*##############################################################################*/

	// buttons for container1 
	containerElements1.push_back(system.createEntity(entity));
	guiToolkit.addButton(containerElements1.back(), functionExampleButton0, buttonText);

	containerElements1.push_back(system.createEntity(entity));
	guiToolkit.addButton(containerElements1.back(), functionExampleButton0, buttonText);

	containerElements1.push_back(system.createEntity(entity));
	guiToolkit.addButton(containerElements1.back(), functionExampleButton0, buttonText);

	containerElements1.push_back(system.createEntity(entity));
	guiToolkit.addButton(containerElements1.back(), functionExampleButton1, buttonText);

	containerElements1.push_back(system.createEntity(entity));
	guiToolkit.addButton(containerElements1.back(), functionExampleButton1, buttonText);

	containerElements1.push_back(system.createEntity(entity));
	guiToolkit.addButton(containerElements1.back(), functionExampleButton1, buttonText);
		
	// textFields for container1
	containerElements1.push_back(system.createEntity(entity));	
	guiToolkit.addTextField(containerElements1.back(), functionExampleTextField, textFieldText);

	//containerElements1.push_back(system.createEntity(entity));
	//guiToolkit.addSlider(containerElements1.back(), glm::vec3(0.f), glm::vec3(1.f), 10, 0);

	containerElements1.push_back(system.createEntity(entity));
	guiToolkit.addCheckBox(containerElements1.back(), glm::vec3(0.4f, 0.91f, 0.f), glm::vec3(0.1f, 0.05f, 1.f), redTexture, greenTexture, functionExampleCheckBox, checkBoxText, glm::vec4(0.f));;

	//container1
	container1 = system.createEntity(container1);
	guiToolkit.addContainer(container1, glm::vec3(0.05f, 0.1f, 0.f), glm::vec3(0.4f, 0.8f, 1.f), 3, 3, 0.05f, lightGrayTexture, true, true, containerElements1);

	/*##############################################################################*/

	// buttons for container2 
	containerElements2.push_back(system.createEntity(entity));
	guiToolkit.addButton(containerElements2.back(), functionExampleButton0, buttonText);

	containerElements2.push_back(system.createEntity(entity));
	guiToolkit.addButton(containerElements2.back(), functionExampleButton1, buttonText);
		
	// textFields for container2
	containerElements2.push_back(system.createEntity(entity));
	guiToolkit.addTextField(containerElements2.back(), functionExampleTextField, textFieldText);

	containerElements2.push_back(system.createEntity(entity));
	guiToolkit.addTextField(containerElements2.back(), functionExampleTextField, textFieldText);

	//container2
	container2 = system.createEntity(container2);
	guiToolkit.addContainer(container2, glm::vec3(0.55f, 0.1f, 0.f), glm::vec3(0.4f, 0.8f, 1.f), 2, 2, 0.05f, true, true, containerElements2);

	/*##############################################################################*/

	// buttons for container3 
	containerElements3.push_back(system.createEntity(entity));
	guiToolkit.addButton(containerElements3.back(), functionExampleButton0, buttonText);

	containerElements3.push_back(system.createEntity(entity));
	guiToolkit.addTextDisplay(containerElements3.back(), textDisplayText);

	// textFields for container3
	containerElements3.push_back(system.createEntity(entity));
	guiToolkit.addTextField(containerElements3.back(), functionExampleTextField, textFieldText);

	// container2 in container3
	containerElements3.push_back(container2);

	//container3
	container3 = system.createEntity(container3);
	guiToolkit.addContainer(container3, glm::vec3(0.55f, 0.1f, 0.f), glm::vec3(0.4f, 0.8f, 1.f), 2, 2, 0.05f, true, true, containerElements3);


}

void ButtonDemo::newState() {
}

void ButtonDemo::update(float _time, float _deltaTime) {

	if (_time > 200) {
		finished = true;
	}
	finished = buttonDemoFinished;
	system.updateTransform(_deltaTime);
	guiToolkit.update(_deltaTime);
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