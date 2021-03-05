#pragma once

#include "../entity_component_system/system.hpp"
#include "../graphics/renderer/fontrenderer.hpp"


//char* defaultText = (char*)"hallo";

class GuiToolkit
{
public:
	GuiToolkit(System& _system);
	
	void addButton(glm::vec3 _position, glm::vec3 _scale, const graphics::Texture2D& _defaultTexture, const graphics::Texture2D& _alternativeTexture, void (*_function)(), char* _text);
	
	inline void addButton(glm::vec3 _position, glm::vec3 _scale, const graphics::Texture2D& _defaultTexture, void (*_function)(), char* _text) {
		addButton(_position, _scale, _defaultTexture, _defaultTexture, _function, _text);
	}

	void updateButton();

	void addTextField(glm::vec3 _position, glm::vec3 _scale, const graphics::Texture2D& _defaultTexture, const graphics::Texture2D& _alternativeTexture, void (*_function)(), char* _text);

	void update();
	
	void updateTextField();

private:
	System& system;
	const graphics::Mesh planeMesh;
	bool pressedButton;
	bool pressedTextField;

	std::string textFieldText;
	char* textFieldTextPointer;

	bool pressedKey_a;
	bool pressedKey_b;
	bool pressedKey_c;
	bool pressedKey_d;
	bool pressedKey_e;
	bool pressedKey_f;
	bool pressedKey_g;
	bool pressedKey_h; 
	bool pressedKey_i;
	bool pressedKey_j;
	bool pressedKey_k;
	bool pressedKey_l;
	bool pressedKey_m;
	bool pressedKey_n;
	bool pressedKey_o;
	bool pressedKey_p;
	bool pressedKey_q;
	bool pressedKey_r;
	bool pressedKey_s;
	bool pressedKey_t;
	bool pressedKey_u;
	bool pressedKey_v;
	bool pressedKey_w;
	bool pressedKey_x;
	bool pressedKey_y;
	bool pressedKey_z;
};