#pragma once
#include "../entity_component_system/components.hpp"
#include "../entity_component_system/system.hpp"

class EntityCreationInterface
{
public:
	static void initialize(System _system);

	static Entity createPlanet(glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale, glm::quat _orientation, glm::vec3 _velocity,
		glm::vec3 _angularVelocity, glm::vec3 _accelaration, float _mass, glm::vec3 _anchor, bool _box_is_projectile);

	static Entity createCrate(glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale, glm::quat _orientation, glm::vec3 _velocity,
		glm::vec3 _angularVelocity, glm::vec3 _accelaration, float _mass, glm::vec3 _anchor, bool _box_is_projectile);

	static Entity createRectangle(const graphics::Texture2D* _texture, glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale);

	static Entity createWorldObject(const graphics::Mesh* _mesh, const graphics::Texture2D* _texture, glm::mat4 _transform, 
		glm::vec3 _position, glm::vec3 _scale, glm::quat _orientation, glm::vec3 _velocity, glm::vec3 _angularVelocity, 
		glm::vec3 _accelaration, float _mass, glm::vec3 _anchor bool _box_is_projectile);

	static std::vector<Entity> createPointLights(float c_constant, float q_constant, float e_constant, std::vector<glm::vec3> positions,
		std::vector<glm::vec3> colors, std::vector<float> intensities);


private:
	System* system;
	static const graphics::Texture2D& texturePlanet;
	static const graphics::Texture2D& textureCratetex;
	static const graphics::Mesh& meshSphere;
	static const graphics::Mesh& meshCrate;
	static const graphics::Mesh& meshRectangle;
}