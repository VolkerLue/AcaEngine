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

Entity EntityCreationInterface::createMovingPlanet(glm::mat4 _transform, glm::vec3 _velocity, glm::vec3 _accelaration) {
	Entity entity;
	system.getEntity(entity);
	system.addMesh(entity, meshSphere);
	system.addTexture(entity, texturePlanet);
	system.addTransform(entity, _transform);
	system.addVelocity(entity, _velocity);
	system.addAcceleration(entity, _accerleration);
	system.addPerspective(entity);
	system.addAABB(true);
	return entity;
}

Entity EntityCreationInterface::createMovingCrate(glm::mat4 _transform, glm::vec3 _velocity, glm::vec3 _accelaration)
{
	Entity entity;
	system.getEntity(entity);
	system.addMesh(entity, meshCrate);
	system.addTexture(entity, textureCratetex);
	system.addTransform(entity, _transform);
	system.addVelocity(entity, _velocity);
	system.addAcceleration(entity, _accerleration);
	system.addPerspective(entity);
	system.addAABB(true);
	return entity;
}

Entity EntityCreationInterface::createSpringPlanet(glm::mat4 _transform, glm::vec3 _position, glm::vec3 _velocity, glm::vec3 _accelaration, 
	float _mass, glm::vec3 _anchor) {
	Entity entity;
	system.getEntity(entity);
	system.addMesh(entity, meshSphere);
	system.addTexture(entity, texturePlanet);
	system.addTransform(entity, _transform);
	system.addPosition(entity, _position);
	system.addVelocity(entity, _velocity);
	system.addAcceleration(entity, _accelaration);
	system.addMass(entity, _mass);
	system.addAnchor(entity, _anchor);
	system.addPerspective(entity);
	system.addAABB(true);
	return entity;
}

Entity EntityCreationInterface::createSpringCrate(glm::mat4 _transform, glm::vec3 _position, glm::vec3 _velocity, glm::vec3 _accelaration,
	float _mass, glm::vec3 _anchor) {
	Entity entity;
	system.getEntity(entity);
	system.addMesh(entity, meshCrate);
	system.addTexture(entity, textureCratetex);
	system.addTransform(entity, _transform);
	system.addPosition(entity, _position);
	system.addVelocity(entity, _velocity);
	system.addAcceleration(entity, _accelaration);
	system.addMass(entity, _mass);
	system.addAnchor(entity, _anchor);
	system.addPerspective(entity);
	system.addAABB(true);
	return entity;
}

Entity EntityCreationInterface::createRotatingPlanet(glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale, glm::quat _orientation, 
	glm::vec3 _velocity, glm::vec3 _angularVelocity, bool _isProjectile) {
	Entity entity;
	system.getEntity(entity);
	system.addMesh(entity, meshSphere);
	system.addTexture(entity, texturePlanet);
	system.addTransform(entity, _transform);
	system.addPosition(entity, _position);
	system.addScale(entity, _scale);
	system.addOrientation(entity, _orientation);
	system.addVelocity(entity, _velocity);
	system.addAngularVelocity(entity, _angularVelocity);
	system.addPerspective(entity);
	system.addAABB(_isProjectile);
	return entity;
}

Entity EntityCreationInterface::createRotatingCrate(glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale, glm::quat _orientation, 
	glm::vec3 _velocity, glm::vec3 _angularVelocity, bool _isProjectile) {
	Entity entity;
	system.getEntity(entity);
	system.addMesh(entity, meshCrate);
	system.addTexture(entity, textureCratetex);
	system.addTransform(entity, _transform);
	system.addPosition(entity, _position);
	system.addScale(entity, _scale);
	system.addOrientation(entity, _orientation);
	system.addVelocity(entity, _velocity);
	system.addAngularVelocity(entity, _angularVelocity);
	system.addPerspective(entity);
	system.addAABB(_isProjectile);
	return entity;
}

Entity EntityCreationInterface::createShootablePlanet(glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale, 
	glm::vec3 _velocity, bool _isProjectile) {
	Entity entity;
	system.getEntity(entity);
	system.addMesh(entity, meshSphere);
	system.addTexture(entity, texturePlanet);
	system.addTransform(entity, _transform);
	system.addPosition(entity, _position);
	system.addScale(entity, _scale);
	system.addVelocity(entity, _velocity);
	system.addPerspective(entity);
	system.addAABB(_isProjectile);
	return entity;
}

Entity EntityCreationInterface::createShootableCrate(glm::mat4 _transform, glm::vec3 _position, glm::vec3 _scale, 
	glm::vec3 _velocity, bool _isProjectile) {
	Entity entity;
	system.getEntity(entity);
	system.addMesh(entity, meshCrate);
	system.addTexture(entity, textureCratetex);
	system.addTransform(entity, _transform);
	system.addPosition(entity, _position);
	system.addScale(entity, _scale);
	system.addVelocity(entity, _velocity);
	system.addPerspective(entity);
	system.addAABB(_isProjectile);
	return entity;
}

static Entity EntityCreationInterface::createPlanet(glm::mat4 _transform, bool _hasPosition, glm::vec3 _position, bool _hasScale, glm::vec3 _scale,
	bool _hasOrientation, glm::quat _orientation, bool _hasVelocity, glm::vec3 _velocity, bool _hasAngularVelocity, glm::vec3 _angularVelocity,
	bool _hasAccelaration, glm::vec3 _accelaration, bool _hasMass, float _mass, bool _hasAnchor, glm::vec3 _anchor,
	bool _hasBox, bool _isProjectile) {
	return createWorldObject(meshSphere, texturePlanet, _transform, _hasPosition, _position, _hasScale, _scale, _hasOrientation, _orientation,
		_hasVelocity, _velocity, _hasAngularVelocity, _angularVelocity, _hasAccelaration, _accelaration, _hasMass, _mass, _hasAnchor, _anchor,
		_hasBox, _isProjectile);
}

static Entity EntityCreationInterface::createCrate(glm::mat4 _transform, bool _hasPosition, glm::vec3 _position, bool _hasScale, glm::vec3 _scale,
	bool _hasOrientation, glm::quat _orientation, bool _hasVelocity, glm::vec3 _velocity, bool _hasAngularVelocity, glm::vec3 _angularVelocity,
	bool _hasAccelaration, glm::vec3 _accelaration, bool _hasMass, float _mass, bool _hasAnchor, glm::vec3 _anchor,
	bool _hasBox, bool _isProjectile) {
	return createWorldObject(meshCratex, textureCrate, _transform, _hasPosition, _position, _hasScale, _scale, _hasOrientation, _orientation,
		_hasVelocity, _velocity, _hasAngularVelocity, _angularVelocity,	_hasAccelaration, _accelaration, _hasMass, _mass, _hasAnchor, _anchor,
		_hasBox, _isProjectile);
}

static Entity EntityCreationInterface::createWorldObject(const graphics::Mesh* _mesh, const graphics::Texture2D* _texture, glm::mat4 _transform,
	bool _hasPosition, glm::vec3 _position, bool _hasScale, glm::vec3 _scale, bool _hasOrientation, glm::quat _orientation, bool _hasVelocity,
	glm::vec3 _velocity, bool _hasAngularVelocity, glm::vec3 _angularVelocity, bool _hasAccelaration, glm::vec3 _accelaration, bool _hasMass, 
	float _mass, bool _hasAnchor, glm::vec3 _anchor, bool _hasBox, bool _isProjectile) {
	Entity entity;
	system.getEntity(entity);
	system.addMesh(entity, _mesh);
	system.addTexture(entity, _texture);
	system.addTransform(entity, _transform);
	if (_hasPosition) system.addPosition(entity, _position);
	if (_hasScale) system.addScale(entity, _scale);
	if (_hasOrientation) system.addOrientation(entity, _orientation);
	if (_hasVelocity) system.addVelocity(entity, _velocity);
	if (_hasAngularVelocity) system.addAngularVelocity(entity, _angularVelocity);
	if (_hasAccelaration) system.addAccelaration(entity, _accelaration);
	if (_hasMass) system.addMass(entity, _mass);
	if (_hasAnchor) system.addAnchor(entity, _anchor);
	if (_hasBox) system.addAABB(entity, _isProjectile);
	system.addPerspective(entity);
	return entity;
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