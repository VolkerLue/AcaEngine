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
	void updateTransform(float _deltaTime);

	void updatePosition(float _deltaTime);

	void updateOrientation(float _deltaTime);
	
	void updateAABB();

	int removeIntersecting();
	
	void shootMeshWithTexure(const graphics::Mesh* _mesh, const graphics::Texture2D& _texture, std::list<Entity>& _entities, float _velocity);
			
	void move(Entity& _entity, float _deltaTime);

	void springY(Entity& _entity, float _deltaTime);


	/* ################ Component-System ################ */
	void addMesh(Entity& _entity, const graphics::Mesh* _mesh);

	void addTransform(Entity& _entity, glm::mat4 _transfrom);

	void setTransform(Entity& _entity, glm::mat4 _transform);

	void addTexture(Entity& _entity, const graphics::Texture2D* _texture);

	void setTexture(Entity& _entity, const graphics::Texture2D* _texture);

	void addPosition(Entity& _entity, glm::vec3 _position);

	void setPosition(Entity& _entity, glm::vec3 _position);

	void addOrientation(Entity& _entity, glm::quat _orientation);

	void setOrientation(Entity& _entity, glm::quat _orientation);

	void addScale(Entity& _entity, glm::vec3 _scale);

	void setScale(Entity& _entity, glm::vec3 _scale);

	void addVelocity(Entity& _entity, glm::vec3 _velocity);

	void setVelocity(Entity& _entity, glm::vec3 _velocity);

	void addAngularVelocity(Entity& _entity, glm::vec3 _angularVelocity);	

	void setAngularVelocity(Entity& _entity, glm::vec3 _angularVelocity);

	void addAccelaration(Entity& _entity, glm::vec3 _velocity);

	void setAccelaration(Entity& _entity, glm::vec3 _accelaration);

	void addMass(Entity& _entity, float _mass);

	void setMass(Entity& _entity, float _mass);

	void addAnchor(Entity& _entity, glm::vec3 _anchor);

	void setAnchor(Entity& _entity, glm::vec3 _anchor);

	void addAABB(Entity& ent, bool isProjectile);	


	/* ################ Utils-System ################ */
	int randomWithoutZero(int quantity, int start);

	int whichEntityIsNotInView();

	template<typename Component>
	bool hasComponent(Entity& _entity) {
		return registry.getComponent<Component>(_entity) != nullptr;
	};

	
private:
	Registry2 registry;
	graphics::Camera camera;
	graphics::MeshRenderer renderer;
	input::InputManager inputManager;	
};
