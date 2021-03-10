#pragma once
#include "../entity_component_system/components.hpp"
#include "../entity_component_system/system.hpp"

class EntityCreationInterface
{
public:
	static void initialize(System _system);


	static Entity createMovingPlanet(glm::mat4 _transform, glm::vec3 _velocity, glm::vec3 _accelaration);

	static Entity createMovingCrate(glm::mat4 _transform, glm::vec3 _velocity, glm::vec3 _accelaration);

	static Entity createSpringPlanet(glm::mat4 _transform, glm::vec3 _position, glm::vec3 _velocity, glm::vec3 _accelaration, float _mass, glm::vec3 _anchor);

	static Entity createSpringCrate(glm::mat4 _transform, glm::vec3 _position, glm::vec3 _velocity, glm::vec3 _accelaration, float _mass, glm::vec3 _anchor);

	static Entity createRotatingPlanet(glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale, glm::quat _orientation, glm::vec3 _velocity,
		glm::vec3 _angularVelocity, bool _isProjectile);

	static Entity createRotatingCrate(glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale, glm::quat _orientation, glm::vec3 _velocity,
		glm::vec3 _angularVelocity, bool _isProjectile);

	static Entity createShootablePlanet(glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale, glm::vec3 _velocity, bool _isProjectile);

	static Entity createShootableCrate(glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale, glm::vec3 _velocity, bool _isProjectile);

	static Entity createPlanet(glm::mat4 _transform, bool _hasPosition, glm::vec3 _position, bool _hasScale, glm::vec3 _scale,
		bool _hasOrientation, glm::quat _orientation, bool _hasVelocity, glm::vec3 _velocity, bool _hasAngularVelocity, glm::vec3 _angularVelocity,
		bool _hasAccelaration, glm::vec3 _accelaration, bool _hasMass, float _mass, bool _hasAnchor, glm::vec3 _anchor,
		bool _hasBox, bool _isProjectile);

	static Entity createCrate(glm::mat4 _transform, bool _hasPosition, glm::vec3 _position, bool _hasScale, glm::vec3 _scale,
		bool _hasOrientation, glm::quat _orientation, bool _hasVelocity, glm::vec3 _velocity, bool _hasAngularVelocity, glm::vec3 _angularVelocity,
		bool _hasAccelaration, glm::vec3 _accelaration, bool _hasMass, float _mass, bool _hasAnchor, glm::vec3 _anchor,
		bool _hasBox, bool _isProjectile);

	static Entity createWorldObject(const graphics::Mesh* _mesh, const graphics::Texture2D* _texture, glm::mat4 _transform,
		bool _hasPosition, glm::vec3 _position, bool _hasScale, glm::vec3 _scale, bool _hasOrientation, glm::quat _orientation,
		bool _hasVelocity, glm::vec3 _velocity, bool _hasAngularVelocity, glm::vec3 _angularVelocity, bool _hasAccelaration,
		glm::vec3 _accelaration, bool _hasMass, float _mass, bool _hasAnchor, glm::vec3 _anchor, bool _hasBox, bool _isProjectile);

	static Entity createRectangle(const graphics::Texture2D* _texture, glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale);

	//For working Point Lights it is necessary, that the objects have AABB
	static std::vector<Entity> createPointLights(float c_constant, float q_constant, float e_constant, std::vector<glm::vec3> positions,
		std::vector<glm::vec3> colors, std::vector<float> intensities);


private:
	System* system;
	static const graphics::Texture2D& texturePlanet;
	static const graphics::Texture2D& textureCratetex;
	static const graphics::Mesh& meshSphere;
	static const graphics::Mesh& meshCrate;
	static const graphics::Mesh& meshRectangle;
};