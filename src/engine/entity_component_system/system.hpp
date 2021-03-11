#pragma once

#include <glm/gtx/transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <limits>
#include <set>
#include <map>

#include "components.hpp"
#include "registry.hpp"
#include "../graphics/renderer/meshrenderer.hpp"
#include "../graphics/renderer/fontrenderer.hpp"
#include "../graphics/core/device.hpp"
#include "../graphics/core/opengl.hpp"
#include "../input/inputmanager.hpp"
#include "../utils/containers/octree.hpp"



class System
{	
public:	
	System();

	/* ################ Entity-System ################ */
	Entity& createEntity(Entity& _entity);

	void eraseEntity(Entity& _entity);

	EntityRef getEntityReference(Entity _ent) const;

	std::optional<Entity> getEntity(EntityRef _ent) const;


	/* ################ Draw-System ################ */	
	void draw();

	void drawEntity(Entity& _entity, const graphics::Texture2D& _texture);

	void setCameraPerspective(float _fov, float _zNear, float zFar);

	void uploadLights(Entity ent);

	void drawText(std::string _text, glm::vec3 _position, float _size, glm::vec4 _color, float _rotation, float _alignX, float _alignY, bool _roundToPixel);


	/* ################ Physic-System ################ */	
	void updateTransform(float _deltaTime);

	void updatePosition(float _deltaTime);

	void updateVelocity(float _deltaTime);

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

	void addAccelaration(Entity& _entity, glm::vec3 _accelaration);

	void setAccelaration(Entity& _entity, glm::vec3 _accelaration);

	void addMass(Entity& _entity, float _mass);

	void setMass(Entity& _entity, float _mass);

	void addAnchor(Entity& _entity, glm::vec3 _anchor);

	void setAnchor(Entity& _entity, glm::vec3 _anchor);

	void addAABB(Entity& ent, bool isProjectile);

	void addPointLight(Entity& ent, glm::vec3 position, glm::vec3 color, float intensity);

	void addLightConstants(float kc, float kq, float ke);

	void setLightConstants(float kc, float kq, float ke);

	void addText(Entity& _entity, char* _text, glm::vec3 _position, float _size, glm::vec4 _color, float _rotation, float _alignX, float _alignY, bool _roundToPixel);

	void setText(Entity& _entity, char* _text, glm::vec3 _position, float _size, glm::vec4 _color, float _rotation, float _alignX, float _alignY, bool _roundToPixel);

	void addOrthogonal(Entity& _entity);

	void addPerspective(Entity& _entity);

	void addAlternativeTexture(Entity& _entity, const graphics::Texture2D* _texture, bool _inUse);

	void addButton(Entity& _entity);

	void addTextField(Entity& _entity, bool _pressed);

	void addFunction(Entity& _entity, void (*_function)(Entity& _entity, System& _system));

	void addSlider(Entity& _entity, Slider _slider);

	void addBox2D(Entity& _entity, glm::vec2 _p1, glm::vec2 _p2);
	
	void addSwitchTexture(Entity& _entity, const graphics::Texture2D* _texture_1, const graphics::Texture2D* _texture_2);

	void setSwitchTexture(Entity& _entity, const graphics::Texture2D* _texture_1, const graphics::Texture2D* _texture_2);
	
	void addBool(Entity& _entity, bool _boolean);

	void setBool(Entity& _entity, bool _boolean);

	bool getBool(Entity& _entity);

	const graphics::Texture2D* getSwitchTexture(Entity& _entity, bool _boolean);


	/* ################ Utils-System ################ */
	int randomWithoutZero(int quantity, int start);

	int whichEntityIsNotInView();

	template<typename Component>
	bool hasComponent(Entity& _entity) {
		return registry.getComponent<Component>(_entity) != nullptr;
	};

	std::unique_ptr<graphics::FontRenderer> fontRenderer;
	Registry registry;		
	graphics::Camera cameraOrthogonal;

	struct Function {
		void (*function)(Entity& _entity, System& _system) {};
	};
private:
	graphics::Camera cameraPerspective;
	graphics::MeshRenderer meshRenderer;	
	input::InputManager inputManager;	
};
