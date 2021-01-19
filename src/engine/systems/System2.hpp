#pragma once

#include "Components.hpp"
#include "../game/Registry2.hpp"
#include "../graphics/core/texture.hpp"
#include <engine/input/inputmanager.hpp>
#include <glm/gtx/transform.hpp>
#include <GL/glew.h>
#include "../game/Octree.hpp"


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
	template<typename... Args>
	void draw(const graphics::Texture2D& _texture);

	void drawEntity(Entity& _entity, const graphics::Texture2D& _texture);

	void setCamera(float _fov, float _zNear, float zFar);

	void repositionCrate();

	void removeIntersecting();

	
	/* ################ Physic-System ################ */	
	void move(Entity& _entity, float _deltaTime);

	void springY(Entity& _entity, float _deltaTime);

	void transfromMultiply(Entity& _entity, glm::mat4 _transform);

	void updateTransformCrate(float _deltaTime);

	void updateTransformPlanet(float _deltaTime);

	void updateShoot(std::vector<Entity> entities);

	void updateAABB();
	
	//void addAngularVelocity(Entity& _entity, glm::vec3 _angular_velocity);
	
	//void rotateMultiply(Entity& _entity);


	/* ################ Component-System ################ */
	void addMesh(Entity& _entity, const char* _mesh);

	void addTransform(Entity& _entity, glm::mat4 _transfrom);

	void setTransform(Entity& _entity, glm::mat4 _transform);

	void addVelocity(Entity& _entity, glm::vec3 _velocity);

	void setVelocity(Entity& _entity, glm::vec3 _velocity);

	void addAccelaration(Entity& _entity, glm::vec3 _velocity);

	void setAccelaration(Entity& _entity, glm::vec3 _accelaration);

	void addMass(Entity& _entity, float _mass);

	void setMass(Entity& _entity, float _mass);

	void addAnchor(Entity& _entity, glm::vec3 _anchor);

	void setAnchor(Entity& _entity, glm::vec3 _anchor);

	void addRotation(Entity& _entity, float _angleInRadians, glm::vec3 _axisOfRotation);
	
	//void addRotation(Entity& _entity, glm::vec3 _eulerAngles);

        //void addAngularVelocity(Entity& _entity, glm::vec3 _angular_velocity);

	void setRotation(Entity& _entity, float _angleInRadians, glm::vec3 _axisOfRotation);

	void addCursorPosition(Entity& _entity, glm::vec3 _curserPosition);

	void setCursorPosition(Entity& _entity, glm::vec3 _curserPosition);

	void addAlive(Entity& _entity, bool _alive);

	void setAlive(Entity& _entity, bool _alive);

	void addAABB(Entity& ent, int type);


	/* ################ Utils-System ################ */
	int randomWithoutZero(int quantity, int start);

	
private:
	Registry2 registry;
	graphics::Camera camera;
	graphics::MeshRenderer renderer;
	input::InputManager inputManager;
	
};


template<typename... Args>
void System2::draw(const graphics::Texture2D& _texture) {
	renderer.clear();
	registry.execute<Args...>([&](const Mesh& mesh, const Transform& transform, const Alive& alive, const Args& args) {
		if (alive.alive) { renderer.draw(mesh.mesh, _texture, transform.transform); }});
	renderer.present(camera);
}
