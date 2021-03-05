#include "guitoolkit.hpp"
#include <iostream>

GuiToolkit::GuiToolkit(System& _system) : 
	system(_system), 
	planeMesh(*utils::MeshLoader::get("models/plane.obj")) 
{
	pressedButton = false;
	pressedTextField = false;

	textFieldText = "";
	textFieldTextPointer = (char*)textFieldText.c_str();

	pressedKey_a = pressedKey_b = pressedKey_c = pressedKey_d =	pressedKey_e = pressedKey_f = pressedKey_g = pressedKey_h = pressedKey_i = pressedKey_j = 
	pressedKey_k = pressedKey_l = pressedKey_m = pressedKey_n = pressedKey_o = pressedKey_p = pressedKey_q = pressedKey_r = pressedKey_s = pressedKey_t = 
	pressedKey_u = pressedKey_v = pressedKey_w = pressedKey_x = pressedKey_y = pressedKey_z = false;

	pressedKey_0 = pressedKey_1 = pressedKey_2 = pressedKey_3 = pressedKey_4 = pressedKey_5 = pressedKey_6 = pressedKey_7 = pressedKey_8 = pressedKey_9 = false;

	pressedKey_backspace = pressedKey_space = false;
}

void GuiToolkit::addButton(glm::vec3 _position, glm::vec3 _scale, const graphics::Texture2D& _defaultTexture, const graphics::Texture2D& _alternativeTexture, void (*_function)(), char* _text) {
	Entity entity;
	system.createEntity(entity);
	system.addMesh(entity, &planeMesh);
	system.addTexture(entity, &_defaultTexture);
	system.addAlternativeTexture(entity, &_alternativeTexture, false);
	system.addTransform(entity, glm::mat4(1.f));
	system.addPosition(entity, _position);
	system.addScale(entity, _scale);
	system.addOrthogonal(entity);	
	system.addButton(entity);
	system.addFunction(entity, _function);
	//system.addAABB(entity, false);

	// correct position and scale for text on button
	float scale = _scale.y;
	math::Rectangle boxButton = math::Rectangle(glm::vec2(_position.x, _position.y), glm::vec2(_position.x + _scale.x, _position.y + _scale.y));
	math::Rectangle boxText = system.fontRenderer->getBoundingBox(glm::vec3(_position.x + 0.01f, _position.y, _position.z), _text, scale, 0.f, 0.f, 0.f, false);
	
	while (boxText.max.x > boxButton.max.x) {
		scale = scale - 0.01;
		boxText = system.fontRenderer->getBoundingBox(glm::vec3(_position.x+0.01f, _position.y, _position.z), _text, scale, 0.f, 0.f, 0.f, false);
	}	
	float centerButtonY = (_position.y + (_position.y + _scale.y)) * 0.5;
	float positionTextY = centerButtonY - (scale * 0.5 );
	system.addText(entity, _text, glm::vec3(_position.x + 0.01f, positionTextY, _position.z), scale, glm::vec4(1.f), 0.f, 0.f, 0.f, false);
}

void GuiToolkit::addTextField(glm::vec3 _position, glm::vec3 _scale, const graphics::Texture2D& _defaultTexture, const graphics::Texture2D& _alternativeTexture, void (*_function)(), char* _text) {
	Entity entity;
	system.createEntity(entity);
	system.addMesh(entity, &planeMesh);
	system.addTexture(entity, &_defaultTexture);
	system.addAlternativeTexture(entity, &_alternativeTexture, false);
	system.addTransform(entity, glm::mat4(1.f));
	system.addPosition(entity, _position);
	system.addScale(entity, _scale);
	system.addOrthogonal(entity);
	system.addTextField(entity);
	system.addFunction(entity, _function);

	// correct position and scale for text on button
	float scale = _scale.y;
	math::Rectangle boxButton = math::Rectangle(glm::vec2(_position.x, _position.y), glm::vec2(_position.x + _scale.x, _position.y + _scale.y));
	math::Rectangle boxText = system.fontRenderer->getBoundingBox(glm::vec3(_position.x + 0.01f, _position.y, _position.z), _text, scale, 0.f, 0.f, 0.f, false);

	while (boxText.max.x > boxButton.max.x) {
		scale = scale - 0.01;
		boxText = system.fontRenderer->getBoundingBox(glm::vec3(_position.x + 0.01f, _position.y, _position.z), _text, scale, 0.f, 0.f, 0.f, false);
	}
	float centerButtonY = (_position.y + (_position.y + _scale.y)) * 0.5;
	float positionTextY = centerButtonY - (scale * 0.5);
	system.addText(entity, _text, glm::vec3(_position.x + 0.01f, positionTextY, _position.z), scale, glm::vec4(0.f), 0.f, 0.f, 0.f, false);
}

void GuiToolkit::updateButton() {
	glm::vec2 cursorPos = system.cameraOrthogonal.toWorldSpace(input::InputManager::getCursorPos());	
	bool leftClick = input::InputManager::isButtonPressed(input::MouseButton::LEFT);

	system.registry.execute<Button, Position, Scale, Texture, AlternativeTexture, Function>([&](
		Button& button, Position& position, Scale& scale, Texture& texture, AlternativeTexture& alternativeTexture, Function& function) {
			math::Rectangle buttonBox = math::Rectangle(glm::vec2(position.position[0], position.position[1]), glm::vec2(position.position[0] + scale.scale[0], position.position[1] + scale.scale[1]));
			
			// change backround if cursor on button
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

			// execute if cursor on botton and leftclick
			if (buttonBox.isIn(cursorPos) == true && leftClick == true && pressedButton == false) {
				pressedButton = true;
				function.function();
			}
			if (leftClick == false) {
				pressedButton = false;
			}
	});
}

void GuiToolkit::updateTextField() {
	glm::vec2 cursorPos = system.cameraOrthogonal.toWorldSpace(input::InputManager::getCursorPos());
	bool leftClick = input::InputManager::isButtonPressed(input::MouseButton::LEFT);

	system.registry.execute<Entity, TextField, Position, Scale, Texture, AlternativeTexture, Text, Function>([&](
		Entity& entity, TextField& textField, Position& position, Scale& scale, Texture& texture, AlternativeTexture& alternativeTexture, Text& text, Function& function) {
			math::Rectangle buttonBox = math::Rectangle(glm::vec2(position.position[0], position.position[1]), glm::vec2(position.position[0] + scale.scale[0], position.position[1] + scale.scale[1]));

			// change backround if cursor on button
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

			// execute if cursor on botton and leftclick
			if (buttonBox.isIn(cursorPos) == true && leftClick == true && pressedTextField == false) {
				pressedTextField = true;			
			}
			if (pressedTextField == true) {				
				using namespace input;
				
				if (InputManager::isKeyPressed(Key::A) && pressedKey_a == false) {
					pressedKey_a = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("A");
					}
					else {
						textFieldText.append("a");
					}					
					textFieldTextPointer = (char*)textFieldText.c_str();
				}	
				if (InputManager::isKeyPressed(Key::A) == false) {
					pressedKey_a = false;
				}

				if (InputManager::isKeyPressed(Key::B) && pressedKey_b == false) {
					pressedKey_b = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("B");
					}
					else {
						textFieldText.append("b");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::B) == false) {
					pressedKey_b = false;
				}

				if (InputManager::isKeyPressed(Key::C) && pressedKey_c == false) {
					pressedKey_c = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("C");
					}
					else {
						textFieldText.append("c");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::C) == false) {
					pressedKey_c = false;
				}

				if (InputManager::isKeyPressed(Key::D) && pressedKey_d == false) {
					pressedKey_d = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("D");
					}
					else {
						textFieldText.append("d");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::D) == false) {
					pressedKey_d = false;
				}

				if (InputManager::isKeyPressed(Key::E) && pressedKey_e == false) {
					pressedKey_e = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("E");
					}
					else {
						textFieldText.append("e");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::E) == false) {
					pressedKey_e = false;
				}

				if (InputManager::isKeyPressed(Key::F) && pressedKey_f == false) {
					pressedKey_f = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("F");
					}
					else {
						textFieldText.append("f");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::F) == false) {
					pressedKey_f = false;
				}

				if (InputManager::isKeyPressed(Key::G) && pressedKey_g == false) {
					pressedKey_g = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("G");
					}
					else {
						textFieldText.append("g");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::G) == false) {
					pressedKey_g = false;
				}

				if (InputManager::isKeyPressed(Key::H) && pressedKey_h == false) {
					pressedKey_h = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("H");
					}
					else {
						textFieldText.append("h");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::H) == false) {
					pressedKey_h = false;
				}

				if (InputManager::isKeyPressed(Key::I) && pressedKey_i == false) {
					pressedKey_i = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("I");
					}
					else {
						textFieldText.append("i");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::I) == false) {
					pressedKey_i = false;
				}

				if (InputManager::isKeyPressed(Key::J) && pressedKey_j == false) {
					pressedKey_j = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("J");
					}
					else {
						textFieldText.append("j");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::J) == false) {
					pressedKey_j = false;
				}

				if (InputManager::isKeyPressed(Key::K) && pressedKey_k == false) {
					pressedKey_k = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("K");
					}
					else {
						textFieldText.append("k");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::K) == false) {
					pressedKey_k = false;
				}

				if (InputManager::isKeyPressed(Key::L) && pressedKey_l == false) {
					pressedKey_l = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("L");
					}
					else {
						textFieldText.append("l");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::L) == false) {
					pressedKey_l = false;
				}

				if (InputManager::isKeyPressed(Key::M) && pressedKey_m == false) {
					pressedKey_m = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("M");
					}
					else {
						textFieldText.append("m");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::M) == false) {
					pressedKey_m = false;
				}

				if (InputManager::isKeyPressed(Key::N) && pressedKey_n == false) {
					pressedKey_n = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("N");
					}
					else {
						textFieldText.append("n");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::N) == false) {
					pressedKey_n = false;
				}

				if (InputManager::isKeyPressed(Key::O) && pressedKey_o == false) {
					pressedKey_o = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("O");
					}
					else {
						textFieldText.append("o");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::O) == false) {
					pressedKey_o = false;
				}

				if (InputManager::isKeyPressed(Key::P) && pressedKey_p == false) {
					pressedKey_p = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("P");
					}
					else {
						textFieldText.append("p");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::P) == false) {
					pressedKey_p = false;
				}

				if (InputManager::isKeyPressed(Key::Q) && pressedKey_q == false) {
					pressedKey_q = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("Q");
					}
					else {
						textFieldText.append("q");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::Q) == false) {
					pressedKey_q = false;
				}

				if (InputManager::isKeyPressed(Key::R) && pressedKey_r == false) {
					pressedKey_r = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("R");
					}
					else {
						textFieldText.append("r");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::R) == false) {
					pressedKey_r = false;
				}

				if (InputManager::isKeyPressed(Key::S) && pressedKey_s == false) {
					pressedKey_s = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("S");
					}
					else {
						textFieldText.append("s");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::S) == false) {
					pressedKey_s = false;
				}

				if (InputManager::isKeyPressed(Key::T) && pressedKey_t == false) {
					pressedKey_t = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("T");
					}
					else {
						textFieldText.append("t");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::T) == false) {
					pressedKey_t = false;
				}

				if (InputManager::isKeyPressed(Key::U) && pressedKey_u == false) {
					pressedKey_u = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("U");
					}
					else {
						textFieldText.append("u");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::U) == false) {
					pressedKey_u = false;
				}

				if (InputManager::isKeyPressed(Key::V) && pressedKey_v == false) {
					pressedKey_v = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("V");
					}
					else {
						textFieldText.append("v");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::V) == false) {
					pressedKey_v = false;
				}

				if (InputManager::isKeyPressed(Key::W) && pressedKey_w == false) {
					pressedKey_w = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("W");
					}
					else {
						textFieldText.append("w");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::W) == false) {
					pressedKey_w = false;
				}

				if (InputManager::isKeyPressed(Key::X) && pressedKey_x == false) {
					pressedKey_x = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("X");
					}
					else {
						textFieldText.append("x");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::X) == false) {
					pressedKey_x = false;
				}

				if (InputManager::isKeyPressed(Key::Y) && pressedKey_y == false) {
					pressedKey_y = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("Y");
					}
					else {
						textFieldText.append("y");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::Y) == false) {
					pressedKey_y = false;
				}

				if (InputManager::isKeyPressed(Key::Z) && pressedKey_z == false) {
					pressedKey_z = true;
					if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
						textFieldText.append("Z");
					}
					else {
						textFieldText.append("z");
					}
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::Z) == false) {
					pressedKey_z = false;
				}

				if (InputManager::isKeyPressed(Key::Num0) && pressedKey_0 == false) {
					pressedKey_0 = true;
					textFieldText.append("0");
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::Num0) == false) {
					pressedKey_0 = false;
				}

				if (InputManager::isKeyPressed(Key::Num1) && pressedKey_1 == false) {
					pressedKey_1 = true;
					textFieldText.append("1");
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::Num1) == false) {
					pressedKey_1 = false;
				}

				if (InputManager::isKeyPressed(Key::Num2) && pressedKey_2 == false) {
					pressedKey_2 = true;
					textFieldText.append("2");
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::Num2) == false) {
					pressedKey_2 = false;
				}

				if (InputManager::isKeyPressed(Key::Num3) && pressedKey_3 == false) {
					pressedKey_3 = true;
					textFieldText.append("3");
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::Num3) == false) {
					pressedKey_3 = false;
				}

				if (InputManager::isKeyPressed(Key::Num4) && pressedKey_4 == false) {
					pressedKey_4 = true;
					textFieldText.append("4");
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::Num4) == false) {
					pressedKey_4 = false;
				}

				if (InputManager::isKeyPressed(Key::Num5) && pressedKey_5 == false) {
					pressedKey_5 = true;
					textFieldText.append("5");
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::Num5) == false) {
					pressedKey_5 = false;
				}

				if (InputManager::isKeyPressed(Key::Num6) && pressedKey_6 == false) {
					pressedKey_6 = true;
					textFieldText.append("6");
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::Num6) == false) {
					pressedKey_6 = false;
				}

				if (InputManager::isKeyPressed(Key::Num7) && pressedKey_7 == false) {
					pressedKey_7 = true;
					textFieldText.append("7");
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::Num7) == false) {
					pressedKey_7 = false;
				}

				if (InputManager::isKeyPressed(Key::Num8) && pressedKey_8 == false) {
					pressedKey_8 = true;
					textFieldText.append("8");
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::Num8) == false) {
					pressedKey_8 = false;
				}

				if (InputManager::isKeyPressed(Key::Num9) && pressedKey_9 == false) {
					pressedKey_9 = true;
					textFieldText.append("9");
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::Num9) == false) {
					pressedKey_9 = false;
				}

				if (InputManager::isKeyPressed(Key::BACKSPACE) && pressedKey_backspace == false) {
					pressedKey_backspace = true;
					textFieldText.pop_back();
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::BACKSPACE) == false) {
					pressedKey_backspace = false;
				}

				if (InputManager::isKeyPressed(Key::SPACE) && pressedKey_space == false) {
					pressedKey_space = true;
					textFieldText.append(" ");
					textFieldTextPointer = (char*)textFieldText.c_str();
				}
				if (InputManager::isKeyPressed(Key::SPACE) == false) {
					pressedKey_space = false;
				}

				//if (InputManager::isKeyPressed(Key::) && pressedKey_space == false) {
				//	pressedKey_space = true;
				//	textFieldText.append(" ");
				//	textFieldTextPointer = (char*)textFieldText.c_str();
				//}
				//if (InputManager::isKeyPressed(Key::SPACE) == false) {
				//	pressedKey_space = false;
				//}

				// correct position and scale for text on button
				float scaleY = scale.scale[1];
				math::Rectangle boxButton = math::Rectangle(glm::vec2(position.position[0], position.position[1]), glm::vec2(position.position[0] + scale.scale[0], position.position[1] + scale.scale[1]));
				math::Rectangle boxText = system.fontRenderer->getBoundingBox(glm::vec3(position.position[0] + 0.01f, position.position[1], position.position[2]), textFieldTextPointer, scaleY, 0.f, 0.f, 0.f, false);

				while (boxText.max.x > boxButton.max.x && scaleY > 0.03) {
					scaleY = scaleY - 0.01f;
					boxText = system.fontRenderer->getBoundingBox(glm::vec3(position.position[0] + 0.01f, position.position[1], position.position[2]), textFieldTextPointer, scaleY, 0.f, 0.f, 0.f, false);
				}
				if (boxText.max.x > boxButton.max.x) {
					textFieldText.pop_back();
					return;
				}
				float centerButtonY = (position.position[1] + (position.position[1] + scale.scale[1])) * 0.5;
				float positionTextY = centerButtonY - (scaleY * 0.5);

				system.registry.removeComponent<Text>(entity);
				system.addText(entity, textFieldTextPointer, glm::vec3(position.position[0] + 0.01f, positionTextY, position.position[2]), scaleY, glm::vec4(0.f), 0.f, 0.f, 0.f, false);
			}
		});
}

void GuiToolkit::update() {
	updateButton();
	updateTextField();
}