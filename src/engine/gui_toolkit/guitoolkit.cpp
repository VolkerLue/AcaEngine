#include "guitoolkit.hpp"

GuiToolkit::GuiToolkit() :
	system(),
	fontRenderer(graphics::FontManager::get("fonts/AnonymousPro.caf")),
	planeMesh(*utils::MeshLoader::get("models/plane.obj")),
	camera(graphics::Camera(45.f, 1.f, 150.f))	
{
	
}

void GuiToolkit::drawText(std::string _text, glm::vec3 _position, float _size, glm::vec4 _color, float _rotation, float _alignX, float _alignY, bool _roundToPixel) {
	fontRenderer->clearText();
	fontRenderer->draw(_position, _text.c_str(), _size, _color, _rotation, _alignX, _alignY, _roundToPixel);
	fontRenderer->present(camera);
}

void GuiToolkit::addButton(glm::vec3 _position, glm::vec3 _scale, const graphics::Texture2D& _planeTexture) {
	Entity entity;
	system.createEntity(entity);
	system.addMesh(entity, &planeMesh);
	system.addTexture(entity, &_planeTexture);
	system.addTransform(entity, glm::mat4(1.f));
	system.addPosition(entity, _position);
	system.addScale(entity, _scale);
	system.addAABB(entity, false);
}

void GuiToolkit::drawButtons(float _deltaTime) {
	system.updateTransform(_deltaTime);
	system.draw();
}