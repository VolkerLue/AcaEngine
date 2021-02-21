#pragma once

#include "../entity_component_system/system.hpp"
#include "../graphics/renderer/fontrenderer.hpp"

class GuiToolkit
{
public:
	GuiToolkit();
	
	void drawText(std::string _text, glm::vec3 _position, float _size, glm::vec4 _color, float _rotation, float _alignX, float _alignY, bool _roundToPixel);

	void addButton(glm::vec3 _position, glm::vec3 _scale, const graphics::Texture2D& _planeTexture);

	void drawButtons(float _deltaTime);

private:
	System system;
	std::unique_ptr<graphics::FontRenderer> fontRenderer;
	graphics::Camera camera;
	const graphics::Mesh planeMesh;
};