#include "entity_creation_interface.hpp"

System* EntityCreationInterface::system = nullptr;
graphics::Texture2D& EntityCreationInterface::texturePlanet = *graphics::Texture2DManager::get("textures/planet1.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR));
graphics::Texture2D& EntityCreationInterface::textureCratetex = *graphics::Texture2DManager::get("textures/cratetex.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR));
graphics::Mesh& EntityCreationInterface::meshSphere = *utils::MeshLoader::get("models/sphere.obj");
graphics::Mesh& EntityCreationInterface::meshCrate = *utils::MeshLoader::get("models/crate.obj");
graphics::Mesh& EntityCreationInterface::meshRectangle = *utils::MeshLoader::get("models/sphere.obj");


EntityCreationInterface::initialize(System _system) {
	system = _system;
}

static Entity EntityCreationInterface::createPlanet(glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale, glm::quat _orientation, 
	glm::vec3 _velocity, glm::vec3 _angularVelocity, glm::vec3 _accelaration, float _mass, glm::vec3 _anchor, bool _box_is_projectile) {
	return createWorldObject(meshSphere, texturePlanet, _transform, _position, _scale, _orientation, _velocity, _angularVelocity,
		 _accelaration, _mass, _anchor, _box_is_projectile);
}

static Entity EntityCreationInterface::createCrate(glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale, glm::quat _orientation,
	glm::vec3 _velocity, glm::vec3 _angularVelocity, glm::vec3 _accelaration, float _mass, glm::vec3 _anchor, bool _box_is_projectile) {
	return createWorldObject(meshCratex, textureCrate, _transform, _position, _scale, _orientation, _velocity, _angularVelocity,
		_accelaration, _mass, _anchor, _box_is_projectile);
}

static Entity EntityCreationInterface::createRectangle(const graphics::Texture2D* _texture, glm::mat4 _transform, glm::vec3 _position,
	glm::vec3 _scale) {
	Entity entity;
	system.getEntity(entity);
	system.addMesh(entity, meshRectangle);
	system.addTexture(entity, _texture);
	system.addTransform(entity, _transform);
	system.addPosition(entity, _position);
	system.addScale(entity, _scale);
	system.addOrthogonal(entity);
	return entity;
}

static Entity EntityCreationInterface::createWorldObject(const graphics::Mesh* _mesh, const graphics::Texture2D* _texture, glm::mat4 _transform,
	glm::vec3 _position, glm::vec3 _scale, glm::quat _orientation, glm::vec3 _velocity, glm::vec3 _angularVelocity, 
	glm::vec3 _accelaration, float _mass, glm::vec3 _anchor bool _box_is_projectile) {
	Entity entity;
	system.getEntity(entity);
	system.addMesh(entity, _mesh);
	system.addTexture(entity, _texture);
	system.addTransform(entity, _transform);
	system.addPosition(entity, _position);
	system.addScale(entity, _scale);
	system.addOrientation(entity, _orientation);
	system.addVelocity(entity, _velocity);
	system.addAngularVelocity(entity, _angularVelocity);
	system.addAccelaration(entity, _accelaration);
	system.addMass(entity, _mass);
	system.addAnchor(entity, _anchor);
	system.addAABB(entity, _box_is_projectile);
	system.addPerspective(entity);
	return entity;
}

static std::vector<Entity> EntityCreationInterface::createPointLights(float c_constant, float q_constant, float e_constant, 
	std::vector<glm::vec3> positions, std::vector<glm::vec3> colors, std::vector<float> intensities) {
	std::vector<Entity> entities;
	system.addLightConstants(c_constant, q_constant, e_constant);
	for (int i = 0; i < positions.size(); i++) {
		Entity entity;
		this->system.getEntity(entity);
		entities.push_back(entity);
		system.addPointLight(entity, position[i], color[i], intensities[i]);
	}
	return entities;
}