#include "settings.hpp"
#include "gamehelper.hpp"
#include <GLFW/glfw3.h>

std::string currentNameFunktionString = "current name: ";
void TextFieldFunktion(Entity& _entity, System& _system) {	
	currentNameFunktionString = "current name: ";
	char* text = _system.registry.getComponentUnsafe<Text>(_entity).text;
	currentNameFunktionString.append(text);
	_system.registry.getComponentUnsafe<Text>(Entity{ 49 }).text = (char*)currentNameFunktionString.c_str();
	_system.registry.getComponentUnsafe<Moved>(Entity{ 49 }).moved = true;

}

void VSyncFunction(Entity& _entity, System& _system) {
	if (_system.registry.getComponentUnsafe<CheckBox>(_entity).status == true) {
		glfwSwapInterval(1); //VSync ON->(1) , OFF->(0)
		_system.registry.getComponentUnsafe<Moved>(_entity).moved = true;
	}
	else {
		glfwSwapInterval(0);
		_system.registry.getComponentUnsafe<Moved>(_entity).moved = true;
	}
}

void backToMainMenu(Entity& _entity, System& _system) {
	choosenGameState = 1;
	gameStateSwitcht = true;
};

Settings::Settings() :
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

	stringSettings		= "Settings";
	stringBackground	= "Background";
	stringColor			= "Color";
	stringHelp			= "Help / More";
	stringName			= "Name Setting";
	stringBack			= "Back";
	stringRed			= "Red";
	stringGreen			= "Green";
	stringBlue			= "Blue";
	stringCurrentName	= "current name: AcaEngine";
	stringChangeName	= "change name";
	stringHelpText1		= "F1    - reset GameState";
	stringHelpText2		= "Left  - prev GameState";
	stringHelpText3		= "Right - next GameState";
	stringHelpText4		= "(if next last, than exit)";
	stringVSync			= "VSync";

	/*##############################################################################*/
	/*                              Settings                                        */
	/*##############################################################################*/
	textDisplaySettingEntity = system.createEntity(textDisplaySettingEntity);
	guiToolkit.addTextDisplay(textDisplaySettingEntity, stringSettings, glm::vec3(0.35f, 0.89f, 0.f), glm::vec3(0.3f, 0.1f, 1.f));
	/*##############################################################################*/
	/*                         Background-Color                                     */
	/*##############################################################################*/
	containerElements1.push_back(system.createEntity(entity));
	guiToolkit.addSlider(containerElements1.back(), glm::vec3(0.f), glm::vec3(1.f), 11, 0, true);
	containerElements1.push_back(system.createEntity(entity));
	guiToolkit.addSlider(containerElements1.back(), glm::vec3(0.f), glm::vec3(1.f), 11, 0, true);
	containerElements1.push_back(system.createEntity(entity));
	guiToolkit.addSlider(containerElements1.back(), glm::vec3(0.f), glm::vec3(1.f), 11, 0, true);
	container1 = system.createEntity(container1);
	guiToolkit.addContainer(container1, glm::vec3(0.f), glm::vec3(1.f), 1, 3, 0.05f, true, false, containerElements1);
	/*##############################################################################*/
	containerElements2.push_back(system.createEntity(entity));
	guiToolkit.addTextDisplay(containerElements2.back(), stringRed, redTexture);
	containerElements2.push_back(system.createEntity(entity));
	guiToolkit.addTextDisplay(containerElements2.back(), stringGreen, greenTexture);
	containerElements2.push_back(system.createEntity(entity));
	guiToolkit.addTextDisplay(containerElements2.back(), stringBlue, lightBlueTexture);
	container2 = system.createEntity(container2);
	guiToolkit.addContainer(container2, glm::vec3(0.f), glm::vec3(1.f), 1, 3, 0.1f, true, false, containerElements2);
	/*##############################################################################*/
	containerElements3.push_back(system.createEntity(entity));
	guiToolkit.addTextDisplay(containerElements3.back(), stringBackground, glm::vec4(1.f, 0.f, 0.f, 1.f));
	containerElements3.push_back(system.createEntity(entity));
	guiToolkit.addTextDisplay(containerElements3.back(), stringColor, glm::vec4(0.f, 0.f, 1.f, 1.f));
	container3 = system.createEntity(container3);
	guiToolkit.addContainer(container3, glm::vec3(0.f), glm::vec3(1.f), 2, 1, 0.f, lightGrayTexture, true, false, containerElements3);
	/*##############################################################################*/
	containerElements4.push_back(container3);
	containerElements4.push_back(container2);
	container4 = system.createEntity(container4);
	guiToolkit.addContainer(container4, glm::vec3(0.f), glm::vec3(1.f), 2, 1, 0.f, lightGrayTexture, true, false, containerElements4);
	/*##############################################################################*/
	containerElements5.push_back(container4);
	containerElements5.push_back(container1);
	container5 = system.createEntity(container5);
	guiToolkit.addContainer(container5, glm::vec3(0.025f, 0.15f, 0.f), glm::vec3(0.3f, 0.7f, 1.f), 2, 1, 0.05f, lightGrayTexture, true, false, containerElements5);
	/*##############################################################################*/
	/*                                  Name                                        */
	/*##############################################################################*/
	containerElements6.push_back(system.createEntity(entity));
	guiToolkit.addTextDisplay(containerElements6.back(), stringName);
	containerElements6.push_back(system.createEntity(entity));
	guiToolkit.addTextField(containerElements6.back(), TextFieldFunktion, stringChangeName);
	containerElements6.push_back(system.createEntity(entity));
	guiToolkit.addTextDisplay(containerElements6.back(), stringCurrentName);
	container6 = system.createEntity(container6);
	guiToolkit.addContainer(container6, glm::vec3(0.35f, 0.15f, 0.f), glm::vec3(0.3f, 0.7f, 1.f), 3, 1, 0.05f, lightGrayTexture, true, false, containerElements6);
	/*##############################################################################*/
	/*                             Help-And-More                                    */
	/*##############################################################################*/
	containerElements7.push_back(system.createEntity(entity));
	guiToolkit.addTextDisplay(containerElements7.back(), stringHelpText1);
	containerElements7.push_back(system.createEntity(entity));
	guiToolkit.addTextDisplay(containerElements7.back(), stringHelpText2);
	containerElements7.push_back(system.createEntity(entity));
	guiToolkit.addTextDisplay(containerElements7.back(), stringHelpText3);
	containerElements7.push_back(system.createEntity(entity));
	guiToolkit.addTextDisplay(containerElements7.back(), stringHelpText4);
	container7 = system.createEntity(container7);
	guiToolkit.addContainer(container7, glm::vec3(0.f), glm::vec3(1.f), 4, 1, 0.1f, lightGrayTexture, true, true, containerElements7);
	/*##############################################################################*/
	containerElements8.push_back(system.createEntity(entity));
	guiToolkit.addTextDisplay(containerElements8.back(), stringHelp);
	containerElements8.push_back(container7);
	containerElements8.push_back(system.createEntity(entity));
	guiToolkit.addCheckBox(containerElements8.back(), VSyncFunction, stringVSync);
	container8 = system.createEntity(container8);
	guiToolkit.addContainer(container8, glm::vec3(0.675f, 0.15f, 0.f), glm::vec3(0.3f, 0.7f, 1.f), 3, 1, 0.05f, lightGrayTexture, true, true, containerElements8);
	/*##############################################################################*/
	/*                            Back                                    */
	/*##############################################################################*/
	buttonBackEntity = system.createEntity(entity);
	guiToolkit.addButton(buttonBackEntity, backToMainMenu, stringBack, glm::vec4(0.f), glm::vec3(0.35f, 0.01f, 0.f), glm::vec3(0.3f, 0.1f, 1.f));
	/*##############################################################################*/
}

void Settings::newState() {
	system.registry.getComponentUnsafe<Position>(container8).position = glm::vec3(0.675f * graphics::Device::getAspectRatio(), 0.15f, 0.f);
	system.registry.getComponentUnsafe<Moved>(container8).moved = true;
	system.registry.getComponentUnsafe<Text>(Entity{ 49 }).text = (char*)stringCurrentName.c_str();
	system.registry.getComponentUnsafe<Moved>(Entity{ 49 }).moved = true;
}

void Settings::update(float _time, float _deltaTime) {
	
	float red = static_cast<float>(guiToolkit.getLevel(containerElements1.at(0))) / 10.f;
	float green = static_cast<float>(guiToolkit.getLevel(containerElements1.at(1))) / 10.f;
	float blue = static_cast<float>(guiToolkit.getLevel(containerElements1.at(2))) / 10.f;
	glClearColor(red, green, blue, 1.f);

	system.updateTransform(_deltaTime);
	guiToolkit.update(_deltaTime);
}

void Settings::draw(float _time, float _deltaTime) {
	system.draw();
}

void Settings::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Settings::onPause(float _time) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Settings::isFinished() {
	return finished;
}