#pragma once
#include "../graphics/renderer/meshrenderer.hpp"
#include "../graphics/core/texture.hpp"

struct Mesh {
	graphics::Mesh mesh;
};

struct Transform {
	glm::mat4 transform;
};

class DrawSystem
{
public:
	DrawSystem() :
		camera(graphics::Camera(45.f, 0.1f, 10000.f)),		
		renderer(graphics::MeshRenderer())
	{
		utils::MeshLoader::clear();
	}

	void addMesh(Entity& _entity, const char* _mesh) {
		EntitySystem::registry.addComponent<Mesh>(_entity, *utils::MeshLoader::get(_mesh));
	}

	void addTransform(Entity& _entity, glm::mat4 _transfrom) {
		EntitySystem::registry.addComponent<Transform>(_entity, _transfrom);
	}

	void setTransform(Entity& _entity, glm::mat4 _transform) {
		Transform& transform = EntitySystem::registry.getComponentUnsafe<Transform>(_entity);
		transform.transform = _transform;
	}

	void draw(Entity& _entity, const graphics::Texture2D& _texture) {
		renderer.clear();
		renderer.draw(EntitySystem::registry.getComponent<Mesh>(_entity)->mesh, _texture, EntitySystem::registry.getComponent<Transform>(_entity)->transform);
		renderer.present(camera);
	}

	void setCamera(float _fov, float _zNear, float zFar) {
		camera = graphics::Camera(_fov, _zNear, zFar);
	}

private:
	graphics::Camera camera;
	graphics::MeshRenderer renderer;
};
