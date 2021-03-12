#pragma once

#include "../entity_component_system/system.hpp"
#include "../graphics/renderer/fontrenderer.hpp"
#include "../graphics/core/device.hpp"


class GuiToolkit
{
public:
	GuiToolkit(System& _system);
	
	/* ################ Button ################ */
	void addButton(Entity& _entity, glm::vec3 _position, glm::vec3 _scale, const graphics::Texture2D& _defaultTexture, const graphics::Texture2D& _alternativeTexture, void (*_function)(Entity& _entity, System& _system), std::string& _text);
		
	inline void addButton(Entity& _entity, glm::vec3 _position, glm::vec3 _scale, const graphics::Texture2D& _defaultTexture, void (*_function)(Entity& _entity, System& _system)) {
		addButton(_entity, _position, _scale, _defaultTexture, _defaultTexture, _function, nullString);
	}

	~GuiToolkit();

	void updateButton();
	

	/* ################ TextField ################ */
	void addTextField(Entity& _entity, glm::vec3 _position, glm::vec3 _scale, const graphics::Texture2D& _defaultTexture, const graphics::Texture2D& _alternativeTexture, void (*_function)(Entity& _entity, System& _system), std::string& _text);
	
	void updateTextField();
		

	/* ################ GUI-Update ################ */
	void update();	


	/* ################ GUI-Utils ################ */
	void findPositionAndScaleForAddText(float& scale, glm::vec3& _scale, glm::vec3& _position, std::string _text, float& positionTextX, float& positionTextY);
	
	bool findPositionAndScaleForTextFieldText(Entity& _entity, float& scaleY, Scale& scale, Position& position, float& positionTextX, float& positionTextY);

	void swapTextureWhenCursorOnIt(Texture& texture, AlternativeTexture& alternativeTexture, math::Rectangle& buttonBox, const glm::vec2& cursorPos);

	void updateKeyInputActions(Entity& _entity, System::Function& _function, TextField& _textField);

	void addSlider(Entity _entity, glm::vec3 _position, glm::vec3 _scale, int _levels, int _selectedLevel, bool vertcal);

	void updateSlider();

	int getLevel(Entity _entity);

private:
	System& system;
	const graphics::Mesh planeMesh;
	bool pressedButton;
	bool pressedTextField;
	const graphics::Texture2D& redTexture;
	const graphics::Texture2D& blackTexture;
	const graphics::Texture2D& greyTexture;

	std::string nullString = "";
	std::map<uint32_t, std::string> textFieldsDefaults;
	std::map<uint32_t, std::string> textFieldsTexts;
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

	bool pressedKey_0;
	bool pressedKey_1;
	bool pressedKey_2;
	bool pressedKey_3;
	bool pressedKey_4;
	bool pressedKey_5;
	bool pressedKey_6;
	bool pressedKey_7;
	bool pressedKey_8;
	bool pressedKey_9;

	bool pressedKey_backspace;
	bool pressedKey_space;
	bool pressedKey_escape;
	bool pressedKey_enter;
};