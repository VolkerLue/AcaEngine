#pragma once
#include "Components.hpp"
#include "../game/Registry2.hpp"
#include "../graphics/core/texture.hpp"
#include <engine/input/inputmanager.hpp>
#include <glm/gtx/transform.hpp>
#include <GL/glew.h>
#include <limits>
#include "../math/geometrictypes.hpp"
#include "../utils/containers/octree.hpp"
#include <set>


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
	void draw();

	void drawEntity(Entity& _entity, const graphics::Texture2D& _texture);

	void setCamera(float _fov, float _zNear, float zFar);


	/* ################ Physic-System ################ */	
	void move(Entity& _entity, float _deltaTime);

	void springY(Entity& _entity, float _deltaTime);

	void transfromMultiply(Entity& _entity, glm::mat4 _transform);

	void updateTransform(float _deltaTime);

	void removeEntityWhenNotInView(std::list<Entity>& entities);

	int whichEntityIsNotInView();

	void shootSphere(std::list<Entity>& _entities, float _velocity, const graphics::Texture2D& _texture);
	
	//void rotate(Entity& _entity, float _deltatime);

	void updateAABB();

	int removeIntersecting();


	/* ################ Component-System ################ */
	void addMesh(Entity& _entity, const char* _mesh);

	void addTransform(Entity& _entity, glm::mat4 _transfrom);

	void setTransform(Entity& _entity, glm::mat4 _transform);

	void addTexture(Entity& _entity, const graphics::Texture2D* _texture);

	void setTexture(Entity& _entity, const graphics::Texture2D* _texture);

	void addVelocity(Entity& _entity, glm::vec3 _velocity);

	void setVelocity(Entity& _entity, glm::vec3 _velocity);

	void addAccelaration(Entity& _entity, glm::vec3 _velocity);

	void setAccelaration(Entity& _entity, glm::vec3 _accelaration);

	void addMass(Entity& _entity, float _mass);

	void setMass(Entity& _entity, float _mass);

	void addAnchor(Entity& _entity, glm::vec3 _anchor);

	void setAnchor(Entity& _entity, glm::vec3 _anchor);

	void addRotation(Entity& _entity, float _angleInRadians, glm::vec3 _axisOfRotation);

	void setRotation(Entity& _entity, float _angleInRadians, glm::vec3 _axisOfRotation);

	//void addRotation(Entity& _entity, glm::vec3 _eulerAngles);

	//void addAngularVelocity(Entity& _entity, glm::vec3 _angular_velocity);

	void addAABB(Entity& ent, bool isProjectile);


	/* ################ Utils-System ################ */
	int randomWithoutZero(int quantity, int start);

	
private:
	Registry2 registry;
	graphics::Camera camera;
	graphics::MeshRenderer renderer;
	input::InputManager inputManager;			

	void executeVelocity(float _deltaTime);

	void executeRotation(float _deltaTime);
};
