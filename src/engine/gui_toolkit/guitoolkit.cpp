#include "guitoolkit.hpp"


GuiToolkit::GuiToolkit(System& _system) : 
	system(_system), 
	planeMesh(*utils::MeshLoader::get("models/plane.obj")),
	redTexture(*graphics::Texture2DManager::get("textures/red.png",
		graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
	blackTexture(*graphics::Texture2DManager::get("textures/black.png",
		graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR)))
{
	pressedButton = false;

	textFieldTextPointer = nullptr;

	pressedKey_a = pressedKey_b = pressedKey_c = pressedKey_d =	pressedKey_e = pressedKey_f = pressedKey_g = pressedKey_h = pressedKey_i = pressedKey_j = 
	pressedKey_k = pressedKey_l = pressedKey_m = pressedKey_n = pressedKey_o = pressedKey_p = pressedKey_q = pressedKey_r = pressedKey_s = pressedKey_t = 
	pressedKey_u = pressedKey_v = pressedKey_w = pressedKey_x = pressedKey_y = pressedKey_z = false;

	pressedKey_0 = pressedKey_1 = pressedKey_2 = pressedKey_3 = pressedKey_4 = pressedKey_5 = pressedKey_6 = pressedKey_7 = pressedKey_8 = pressedKey_9 = false;

	pressedKey_backspace = pressedKey_space = pressedKey_escape = pressedKey_enter = false;
}


/* ################ Button ################ */
void GuiToolkit::addButton(Entity& _entity, glm::vec3 _position, glm::vec3 _scale, const graphics::Texture2D& _defaultTexture, const graphics::Texture2D& _alternativeTexture, void (*_function)(Entity& _entity, System& _system), std::string& _text)
{
	_position.x *= graphics::Device::getAspectRatio();
	_scale.x *= graphics::Device::getAspectRatio();

	system.addMesh(_entity, &planeMesh);
	system.addTexture(_entity, &_defaultTexture);
	system.addAlternativeTexture(_entity, &_alternativeTexture, false);
	system.addTransform(_entity, glm::mat4(1.f));
	system.addPosition(_entity, _position);
	system.addScale(_entity, _scale);
	system.addOrthogonal(_entity);
	system.addButton(_entity);
	system.addFunction(_entity, _function);

	float positionTextX, positionTextY, scaleY = 0.0f;
	findPositionAndScaleForAddText(scaleY, _scale, _position, _text, positionTextX, positionTextY);
	system.addText(_entity, (char*)_text.c_str(), glm::vec3(positionTextX, positionTextY, _position.z), scaleY, glm::vec4(1.f), 0.f, 0.f, 0.f, false);
}

GuiToolkit::~GuiToolkit() {
	system.registry.execute<Slider>([&](Slider& slider) {
		delete[] slider.levelEntities;
		});
}

void GuiToolkit::updateButton() {
	glm::vec2 cursorPos = system.cameraOrthogonal.toWorldSpace(input::InputManager::getCursorPos());	
	bool leftClick = input::InputManager::isButtonPressed(input::MouseButton::LEFT);

	system.registry.execute<Entity, Button, Position, Scale, Texture, AlternativeTexture, System::Function>([&](
		Entity& entity, Button& button, Position& position, Scale& scale, Texture& texture, AlternativeTexture& alternativeTexture, System::Function& function) {
			math::Rectangle buttonBox = math::Rectangle(glm::vec2(position.position[0], position.position[1]), glm::vec2(position.position[0] + scale.scale[0], position.position[1] + scale.scale[1]));

			swapTextureWhenCursorOnIt(texture, alternativeTexture, buttonBox, cursorPos);

			if (buttonBox.isIn(cursorPos) == true && leftClick == true && pressedButton == false) {
				pressedButton = true;
				function.function(entity, system);
			}
			if (leftClick == false) {
				pressedButton = false;
			}
		});
}


/* ################ TextField ################ */
void GuiToolkit::addTextField(Entity& _entity, glm::vec3 _position, glm::vec3 _scale, const graphics::Texture2D& _defaultTexture, const graphics::Texture2D& _alternativeTexture, void (*_function)(Entity& _entity, System& _system), std::string& _text)
{
	_position.x *= graphics::Device::getAspectRatio();
	_scale.x *= graphics::Device::getAspectRatio();

	system.addMesh(_entity, &planeMesh);
	system.addTexture(_entity, &_defaultTexture);
	system.addAlternativeTexture(_entity, &_alternativeTexture, false);
	system.addTransform(_entity, glm::mat4(1.f));
	system.addPosition(_entity, _position);
	system.addScale(_entity, _scale);
	system.addOrthogonal(_entity);
	system.addTextField(_entity, false);
	system.addFunction(_entity, _function);

	textFieldsDefaults[_entity.id] = _text;
	textFieldsTexts[_entity.id] = nullString;

	float positionTextX, positionTextY, scale = 0.f;
	findPositionAndScaleForAddText(scale, _scale, _position, _text, positionTextX, positionTextY);
	system.addText(_entity, (char*)_text.c_str(), glm::vec3(positionTextX, positionTextY, _position.z), scale, glm::vec4(0.f), 0.f, 0.f, 0.f, false);
}

void GuiToolkit::updateTextField() 
{
	glm::vec2 cursorPos = system.cameraOrthogonal.toWorldSpace(input::InputManager::getCursorPos());
	bool leftClick = input::InputManager::isButtonPressed(input::MouseButton::LEFT);

	system.registry.execute<Entity, TextField, Position, Scale, Texture, AlternativeTexture, Text, System::Function>([&](
		Entity& entity, TextField& textField, Position& position, Scale& scale, Texture& texture, AlternativeTexture& alternativeTexture, Text& text, System::Function& function) {
			math::Rectangle buttonBox = math::Rectangle(glm::vec2(position.position[0], position.position[1]), glm::vec2(position.position[0] + scale.scale[0], position.position[1] + scale.scale[1]));

			swapTextureWhenCursorOnIt(texture, alternativeTexture, buttonBox, cursorPos);

			if (buttonBox.isIn(cursorPos) == true && leftClick == true && textField.pressed == false) {
				textField.pressed = true;
			}
			if (buttonBox.isIn(cursorPos) == false && leftClick == true && textField.pressed == true) {
				textField.pressed = false;
			}
			if (textField.pressed == true) {
				if (textFieldsTexts[entity.id] == nullString) {
					text.text = (char*)textFieldsTexts[entity.id].c_str();
				}
				std::string tempString = textFieldsTexts[entity.id];
				updateKeyInputActions(entity, function, textField);
				if (tempString != textFieldsTexts[entity.id]) {
					float positionTextX, positionTextY, scaleY = 0.f;
					if (findPositionAndScaleForTextFieldText(entity, scaleY, scale, position, positionTextX, positionTextY)) {
						text.text = textFieldTextPointer;
						text.position.x = positionTextX;
						text.position.y = positionTextY;
						text.size = scaleY;						
					}
				}
			}
		});
}


/* ################ GUI-Update ################ */
void GuiToolkit::update() 
{
	updateButton();
	updateTextField();
	updateSlider();
}


/* ################ GUI-Utils ################ */
void GuiToolkit::findPositionAndScaleForAddText(float& scaleY, glm::vec3& _scale, glm::vec3& _position, std::string _text, float& positionTextX, float& positionTextY)
{
	scaleY = _scale.y;
	math::Rectangle boxButton = math::Rectangle(glm::vec2(_position.x, _position.y), glm::vec2(_position.x + _scale.x, _position.y + _scale.y));
	math::Rectangle boxText = system.fontRenderer->getBoundingBox(glm::vec3(_position.x, _position.y, _position.z), _text.c_str(), scaleY, 0.f, 0.f, 0.f, false);

	auto windowSize = graphics::Device::getBufferSize();
	float scaleWindow = 0.f;
	if (windowSize.x < windowSize.y) {
		scaleWindow = static_cast<float>(windowSize.x) / 1000.f;
	}
	else scaleWindow = static_cast<float>(windowSize.y) / 1000.f;

	while (boxText.max.x > boxButton.max.x && scaleY > (0.015f / scaleWindow) ) {
		scaleY = scaleY - 0.001f;
		boxText = system.fontRenderer->getBoundingBox(glm::vec3(_position.x, _position.y, _position.z), _text.c_str(), scaleY, 0.f, 0.f, 0.f, false);
	}
	while (boxText.max.x > boxButton.max.x) {
		if (_text.size() > 0) {
			_text.pop_back();
			boxText = system.fontRenderer->getBoundingBox(glm::vec3(_position.x, _position.y, _position.z), _text.c_str(), scaleY, 0.f, 0.f, 0.f, false);
		}
	}
	float centerButtonX = (_position.x + (_position.x + _scale.x)) * 0.5f;
	positionTextX = centerButtonX - ((boxText.max.x - boxText.min.x) * 0.5f);
	float centerButtonY = (_position.y + (_position.y + _scale.y)) * 0.5f;
	positionTextY = centerButtonY - (scaleY * 0.5f);
}

bool GuiToolkit::findPositionAndScaleForTextFieldText(Entity& _entity, float& scaleY, Scale& scale, Position& position, float& positionTextX, float& positionTextY)
{
	scaleY = scale.scale[1];
	math::Rectangle boxButton = math::Rectangle(glm::vec2(position.position[0], position.position[1]), glm::vec2(position.position[0] + scale.scale[0], position.position[1] + scale.scale[1]));
	math::Rectangle boxText = system.fontRenderer->getBoundingBox(glm::vec3(position.position[0], position.position[1], position.position[2]), textFieldTextPointer, scaleY, 0.f, 0.f, 0.f, false);

	auto windowSize = graphics::Device::getBufferSize();
	float scaleWindow = 0;
	if (windowSize.x < windowSize.y) {
		scaleWindow = static_cast<float>(windowSize.x) / 1000;
	}
	else scaleWindow = static_cast<float>(windowSize.y) / 1000;

	while (boxText.max.x > boxButton.max.x && scaleY > (0.015 / scaleWindow)) {
		scaleY = scaleY - 0.001f;
		boxText = system.fontRenderer->getBoundingBox(glm::vec3(position.position[0], position.position[1], position.position[2]), textFieldTextPointer, scaleY, 0.f, 0.f, 0.f, false);
	}
	if (boxText.max.x > boxButton.max.x) {
		textFieldsTexts[_entity.id].pop_back();
		return false;
	}
	float centerButtonX = (position.position[0] + (position.position[0] + scale.scale[0])) * 0.5;
	positionTextX = centerButtonX - ((boxText.max.x - boxText.min.x) * 0.5f);
	float centerButtonY = (position.position[1] + (position.position[1] + scale.scale[1])) * 0.5;
	positionTextY = centerButtonY - (scaleY * 0.5);
	return true;
}

void GuiToolkit::swapTextureWhenCursorOnIt(Texture& texture, AlternativeTexture& alternativeTexture, math::Rectangle& buttonBox, const glm::vec2& cursorPos)
{
	if (texture.texture != alternativeTexture.texture) {
		if (buttonBox.isIn(cursorPos) == true && alternativeTexture.inUse == false) {
			alternativeTexture.inUse = true;
			const graphics::Texture2D& tempTexture = *texture.texture;
			texture.texture = alternativeTexture.texture;
			alternativeTexture.texture = &tempTexture;
		}
		if (buttonBox.isIn(cursorPos) == false && alternativeTexture.inUse == true) {
			alternativeTexture.inUse = false;
			const graphics::Texture2D& tempTexture = *texture.texture;
			texture.texture = alternativeTexture.texture;
			alternativeTexture.texture = &tempTexture;
		}
	}
}

void GuiToolkit::updateKeyInputActions(Entity& _entity, System::Function& _function, TextField& _textField)
{
	using namespace input;

	if (InputManager::isKeyPressed(Key::A) && pressedKey_a == false) {
		pressedKey_a = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("A");
		}
		else {
			textFieldsTexts[_entity.id].append("a");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::A) == false && pressedKey_a == true) {
		pressedKey_a = false;
	}

	else if (InputManager::isKeyPressed(Key::B) && pressedKey_b == false) {
		pressedKey_b = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("B");
		}
		else {
			textFieldsTexts[_entity.id].append("b");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::B) == false && pressedKey_b == true) {
		pressedKey_b = false;
	}

	else if (InputManager::isKeyPressed(Key::C) && pressedKey_c == false) {
		pressedKey_c = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("C");
		}
		else {
			textFieldsTexts[_entity.id].append("c");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::C) == false && pressedKey_c == true) {
		pressedKey_c = false;
	}

	else if (InputManager::isKeyPressed(Key::D) && pressedKey_d == false) {
		pressedKey_d = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("D");
		}
		else {
			textFieldsTexts[_entity.id].append("d");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::D) == false && pressedKey_d == true) {
		pressedKey_d = false;
	}

	else if (InputManager::isKeyPressed(Key::E) && pressedKey_e == false) {
		pressedKey_e = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("E");
		}
		else {
			textFieldsTexts[_entity.id].append("e");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::E) == false && pressedKey_e == true) {
		pressedKey_e = false;
	}

	else if (InputManager::isKeyPressed(Key::F) && pressedKey_f == false) {
		pressedKey_f = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("F");
		}
		else {
			textFieldsTexts[_entity.id].append("f");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::F) == false && pressedKey_f == true) {
		pressedKey_f = false;
	}

	else if (InputManager::isKeyPressed(Key::G) && pressedKey_g == false) {
		pressedKey_g = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("G");
		}
		else {
			textFieldsTexts[_entity.id].append("g");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::G) == false && pressedKey_g == true) {
		pressedKey_g = false;
	}

	else if (InputManager::isKeyPressed(Key::H) && pressedKey_h == false) {
		pressedKey_h = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("H");
		}
		else {
			textFieldsTexts[_entity.id].append("h");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::H) == false && pressedKey_h == true) {
		pressedKey_h = false;
	}

	else if (InputManager::isKeyPressed(Key::I) && pressedKey_i == false) {
		pressedKey_i = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("I");
		}
		else {
			textFieldsTexts[_entity.id].append("i");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::I) == false && pressedKey_i == true) {
		pressedKey_i = false;
	}

	else if (InputManager::isKeyPressed(Key::J) && pressedKey_j == false) {
		pressedKey_j = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("J");
		}
		else {
			textFieldsTexts[_entity.id].append("j");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::J) == false && pressedKey_j == true) {
		pressedKey_j = false;
	}

	else if (InputManager::isKeyPressed(Key::K) && pressedKey_k == false) {
		pressedKey_k = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("K");
		}
		else {
			textFieldsTexts[_entity.id].append("k");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::K) == false && pressedKey_k == true) {
		pressedKey_k = false;
	}

	else if (InputManager::isKeyPressed(Key::L) && pressedKey_l == false) {
		pressedKey_l = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("L");
		}
		else {
			textFieldsTexts[_entity.id].append("l");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::L) == false && pressedKey_l == true) {
		pressedKey_l = false;
	}

	else if (InputManager::isKeyPressed(Key::M) && pressedKey_m == false) {
		pressedKey_m = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("M");
		}
		else {
			textFieldsTexts[_entity.id].append("m");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::M) == false && pressedKey_m == true) {
		pressedKey_m = false;
	}

	else if (InputManager::isKeyPressed(Key::N) && pressedKey_n == false) {
		pressedKey_n = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("N");
		}
		else {
			textFieldsTexts[_entity.id].append("n");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::N) == false && pressedKey_n == true) {
		pressedKey_n = false;
	}

	else if (InputManager::isKeyPressed(Key::O) && pressedKey_o == false) {
		pressedKey_o = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("O");
		}
		else {
			textFieldsTexts[_entity.id].append("o");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::O) == false && pressedKey_o == true) {
		pressedKey_o = false;
	}

	else if (InputManager::isKeyPressed(Key::P) && pressedKey_p == false) {
		pressedKey_p = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("P");
		}
		else {
			textFieldsTexts[_entity.id].append("p");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::P) == false && pressedKey_p == true) {
		pressedKey_p = false;
	}

	else if (InputManager::isKeyPressed(Key::Q) && pressedKey_q == false) {
		pressedKey_q = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("Q");
		}
		else {
			textFieldsTexts[_entity.id].append("q");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Q) == false && pressedKey_q == true) {
		pressedKey_q = false;
	}

	else if (InputManager::isKeyPressed(Key::R) && pressedKey_r == false) {
		pressedKey_r = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("R");
		}
		else {
			textFieldsTexts[_entity.id].append("r");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::R) == false && pressedKey_r == true) {
		pressedKey_r = false;
	}

	else if (InputManager::isKeyPressed(Key::S) && pressedKey_s == false) {
		pressedKey_s = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("S");
		}
		else {
			textFieldsTexts[_entity.id].append("s");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::S) == false && pressedKey_s == true) {
		pressedKey_s = false;
	}

	else if (InputManager::isKeyPressed(Key::T) && pressedKey_t == false) {
		pressedKey_t = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("T");
		}
		else {
			textFieldsTexts[_entity.id].append("t");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::T) == false && pressedKey_t == true) {
		pressedKey_t = false;
	}

	else if (InputManager::isKeyPressed(Key::U) && pressedKey_u == false) {
		pressedKey_u = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("U");
		}
		else {
			textFieldsTexts[_entity.id].append("u");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::U) == false && pressedKey_u == true) {
		pressedKey_u = false;
	}

	else if (InputManager::isKeyPressed(Key::V) && pressedKey_v == false) {
		pressedKey_v = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("V");
		}
		else {
			textFieldsTexts[_entity.id].append("v");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::V) == false && pressedKey_v == true) {
		pressedKey_v = false;
	}

	else if (InputManager::isKeyPressed(Key::W) && pressedKey_w == false) {
		pressedKey_w = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("W");
		}
		else {
			textFieldsTexts[_entity.id].append("w");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::W) == false && pressedKey_w == true) {
		pressedKey_w = false;
	}

	else if (InputManager::isKeyPressed(Key::X) && pressedKey_x == false) {
		pressedKey_x = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("X");
		}
		else {
			textFieldsTexts[_entity.id].append("x");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::X) == false && pressedKey_x == true) {
		pressedKey_x = false;
	}

	else if (InputManager::isKeyPressed(Key::Y) && pressedKey_y == false) {
		pressedKey_y = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("Y");
		}
		else {
			textFieldsTexts[_entity.id].append("y");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Y) == false && pressedKey_y == true) {
		pressedKey_y = false;
	}

	else if (InputManager::isKeyPressed(Key::Z) && pressedKey_z == false) {
		pressedKey_z = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("Z");
		}
		else {
			textFieldsTexts[_entity.id].append("z");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Z) == false && pressedKey_z == true) {
		pressedKey_z = false;
	}

	else if (InputManager::isKeyPressed(Key::Num0) && pressedKey_0 == false) {
		pressedKey_0 = true;
		textFieldsTexts[_entity.id].append("0");
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Num0) == false && pressedKey_0 == true) {
		pressedKey_0 = false;
	}

	else if (InputManager::isKeyPressed(Key::Num1) && pressedKey_1 == false) {
		pressedKey_1 = true;
		textFieldsTexts[_entity.id].append("1");
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Num1) == false && pressedKey_1 == true) {
		pressedKey_1 = false;
	}

	else if (InputManager::isKeyPressed(Key::Num2) && pressedKey_2 == false) {
		pressedKey_2 = true;
		textFieldsTexts[_entity.id].append("2");
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Num2) == false && pressedKey_2 == true) {
		pressedKey_2 = false;
	}

	else if (InputManager::isKeyPressed(Key::Num3) && pressedKey_3 == false) {
		pressedKey_3 = true;
		textFieldsTexts[_entity.id].append("3");
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Num3) == false && pressedKey_3 == true) {
		pressedKey_3 = false;
	}

	else if (InputManager::isKeyPressed(Key::Num4) && pressedKey_4 == false) {
		pressedKey_4 = true;
		textFieldsTexts[_entity.id].append("4");
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Num4) == false && pressedKey_4 == true) {
		pressedKey_4 = false;
	}

	else if (InputManager::isKeyPressed(Key::Num5) && pressedKey_5 == false) {
		pressedKey_5 = true;
		textFieldsTexts[_entity.id].append("5");
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Num5) == false && pressedKey_5 == true) {
		pressedKey_5 = false;
	}

	else if (InputManager::isKeyPressed(Key::Num6) && pressedKey_6 == false) {
		pressedKey_6 = true;
		textFieldsTexts[_entity.id].append("6");
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Num6) == false && pressedKey_6 == true) {
		pressedKey_6 = false;
	}

	else if (InputManager::isKeyPressed(Key::Num7) && pressedKey_7 == false) {
		pressedKey_7 = true;
		textFieldsTexts[_entity.id].append("7");
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Num7) == false && pressedKey_7 == true) {
		pressedKey_7 = false;
	}

	else if (InputManager::isKeyPressed(Key::Num8) && pressedKey_8 == false) {
		pressedKey_8 = true;
		textFieldsTexts[_entity.id].append("8");
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Num8) == false && pressedKey_8 == true) {
		pressedKey_8 = false;
	}

	else if (InputManager::isKeyPressed(Key::Num9) && pressedKey_9 == false) {
		pressedKey_9 = true;
		textFieldsTexts[_entity.id].append("9");
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Num9) == false && pressedKey_9 == true) {
		pressedKey_9 = false;
	}

	else if (InputManager::isKeyPressed(Key::BACKSPACE) && pressedKey_backspace == false) {
		pressedKey_backspace = true;
		if (textFieldsTexts[_entity.id].size() > 0) {
			textFieldsTexts[_entity.id].pop_back();
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::BACKSPACE) == false && pressedKey_backspace == true) {
		pressedKey_backspace = false;
	}

	else if (InputManager::isKeyPressed(Key::SPACE) && pressedKey_space == false) {
		pressedKey_space = true;
		textFieldsTexts[_entity.id].append(" ");
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::SPACE) == false && pressedKey_space == true) {
		pressedKey_space = false;
	}

	else if (InputManager::isKeyPressed(Key::ESCAPE) && pressedKey_escape == false) {
	pressedKey_escape = true;
	textFieldsTexts[_entity.id].erase();
	textFieldsTexts[_entity.id].shrink_to_fit();
	textFieldTextPointer = (char*)textFieldsDefaults[_entity.id].c_str();
	_textField.pressed = false;
	}
	else if (InputManager::isKeyPressed(Key::ESCAPE) == false && pressedKey_escape == true) {
	pressedKey_escape = false;
	}

	else if (InputManager::isKeyPressed(Key::ENTER) && pressedKey_enter == false) {
	pressedKey_enter = true;
	Text text;
	text.text = (char*)textFieldsTexts[_entity.id].c_str();
	_function.function(_entity, system);
	textFieldsTexts[_entity.id].erase();
	textFieldsTexts[_entity.id].shrink_to_fit();
	textFieldTextPointer = (char*)textFieldsDefaults[_entity.id].c_str();
	_textField.pressed = false;;
	}
	else if (InputManager::isKeyPressed(Key::ENTER) == false && pressedKey_enter == true) {
	pressedKey_enter = false;
	}

	
}
void GuiToolkit::addSlider(Entity _entity, glm::vec3 _position, glm::vec3 _scale, int _levels, int _selectedLevel) {
	Entity* levelEntities = new Entity[_levels];
	Slider slider = { _entity, levelEntities, _selectedLevel, _levels };
	system.addMesh(_entity, &planeMesh);
	system.addTexture(_entity, graphics::Texture2DManager::get("textures/lightGray.png",
		graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR)));
	system.addTransform(_entity, glm::mat4(1.f));
	system.addPosition(_entity, _position);
	system.addScale(_entity, _scale);
	system.addOrthogonal(_entity);
	slider.backgroundEntity = _entity;
	float sizeLevel = _scale.y / _levels * 0.8f;
	float sizeLevelDistance = _scale.y / (_levels - 1) * 0.2f;
	for (int i = 0; i < _levels; i++) {
		Entity levelEntity;
		system.createEntity(levelEntity);
		system.addMesh(levelEntity, &planeMesh);
		system.addTransform(levelEntity, glm::mat4(1.f));
		glm::vec3 pos = glm::vec3(0.f, i * (sizeLevel + sizeLevelDistance), 0.f) + _position;
		system.addPosition(levelEntity, pos);
		glm::vec3 sca = glm::vec3(1.f, 1.f / _levels * 0.8, 1.f) * _scale;
		system.addScale(levelEntity, sca);
		system.addBox2D(levelEntity, glm::vec2(pos.x, pos.y), glm::vec2(pos.x + sca.x, pos.y + sca.y));
		system.addOrthogonal(levelEntity);
		if (i == _selectedLevel) {
			system.addTexture(levelEntity, &redTexture);
			slider.currentLevel = i;
		}
		else {
			system.addTexture(levelEntity, &blackTexture);
		}
		slider.levelEntities[i] = levelEntity;
	}
	system.addSlider(_entity, slider);
}

void GuiToolkit::updateSlider() {
	bool isPressed = input::InputManager::isButtonPressed(input::MouseButton::LEFT);
	glm::vec2 cursorPos = system.cameraOrthogonal.toWorldSpace(input::InputManager::getCursorPos());
	system.registry.execute<Slider, Position, Scale>([&](Slider& slider, Position& position, Scale& scale) {
		if (isPressed) {
			for (int i = 0; i < slider.numberOfLevels; i++) {
				math::Rectangle currentBox = system.registry.getComponentUnsafe<Box2D>(slider.levelEntities[i]).box;
				if (currentBox.isIn(cursorPos)) {
					system.setTexture(slider.levelEntities[slider.currentLevel], &blackTexture);
					system.setTexture(slider.levelEntities[i], &redTexture);
					slider.currentLevel = i;
				}
			}
		}
		});
}

int GuiToolkit::getLevel(Entity _entity) {
	return system.registry.getComponentUnsafe<Slider>(_entity).currentLevel;
}