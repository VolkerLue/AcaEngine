#pragma once

#include "Components.hpp"
#include "../game/Registry2.hpp"
#include "../graphics/core/texture.hpp"

class System2
{	
public:	
	System2();

	/* ################ Entity-System ################ */
	Entity& createEntity(Entity& _entity);

	void eraseEntity(Entity& _entity);

	EntityRef getEntityReference(Entity _ent) const;

	std::optional<Entity> getEntity(EntityRef _ent) const;

	/* ################ Draw-System ################ */
	void addMesh(Entity& _entity, const char* _mesh);

	void addTransform(Entity& _entity, glm::mat4 _transfrom);

	void setTransform(Entity& _entity, glm::mat4 _transform);

	void transfromMultiply(Entity& _entity, glm::mat4 _transform);

	void drawEntity(Entity& _entity, const graphics::Texture2D& _texture);

	template<typename... Args>
	void draw(const graphics::Texture2D& _texture);

	void setCamera(float _fov, float _zNear, float zFar);

	/* ################ Physic-System ################ */
	void addVelocity(Entity& _entity, glm::vec3 _velocity);

	void setVelocity(Entity& _entity, glm::vec3 _velocity);

	void addAccelaration(Entity& _entity, glm::vec3 _velocity);

	void setAccelaration(Entity& _entity, glm::vec3 _accelaration);

	void addMass(Entity& _entity, float _mass);

	void setMass(Entity& _entity, float _mass);

	void addAnchor(Entity& _entity, glm::vec3 _anchor);

	void setAnchor(Entity& _entity, glm::vec3 _anchor);

	void move(Entity& _entity, float _deltaTime);

	void springY(Entity& _entity, float _deltaTime);
	
private:
	Registry2 registry;
	graphics::Camera camera;
	graphics::MeshRenderer renderer;
};

template<typename... Args>
void System2::draw(const graphics::Texture2D& _texture) {
	renderer.clear();
	registry.execute<Args...>([&](const Mesh& mesh, const Transform& transform, const Args& args) {
		renderer.draw(mesh.mesh, _texture, transform.transform); });
	renderer.present(camera);
}
