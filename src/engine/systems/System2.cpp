#include "System2.hpp"

System2::System2() : registry(),
camera(graphics::Camera(45.f, 0.1f, 10000.f)),
renderer(graphics::MeshRenderer())
{
	utils::MeshLoader::clear();
}

/* ################ Entity-System ################ */
Entity& System2::createEntity(Entity& entity) {
	return entity = registry.create();
}

void System2::eraseEntity(Entity& _entity) {
	registry.erase(_entity);
}

EntityRef System2::getEntityReference(Entity _entity) const {
	return registry.getRef(_entity);
}

std::optional<Entity> System2::getEntity(EntityRef _entity) const {
	return registry.getEntity(_entity);
}

/* ################ Draw-System ################ */
void System2::addMesh(Entity& _entity, const char* _mesh) {
	registry.addComponent<Mesh>(_entity, *utils::MeshLoader::get(_mesh));
}

void System2::addTransform(Entity& _entity, glm::mat4 _transfrom) {
	registry.addComponent<Transform>(_entity, _transfrom);
}

void System2::setTransform(Entity& _entity, glm::mat4 _transform) {
	Transform& transform = registry.getComponentUnsafe<Transform>(_entity);
	transform.transform = _transform;
}

void System2::transfromMultiply(Entity& _entity, glm::mat4 _transform) {
	Transform& transform = registry.getComponentUnsafe<Transform>(_entity);
	transform.transform *= _transform;
}

void System2::drawEntity(Entity& _entity, const graphics::Texture2D& _texture) {
	renderer.clear();
	renderer.draw(registry.getComponent<Mesh>(_entity)->mesh, _texture, registry.getComponent<Transform>(_entity)->transform);
	renderer.present(camera);
}

void System2::setCamera(float _fov, float _zNear, float zFar) {
	camera = graphics::Camera(_fov, _zNear, zFar);
}

/* ################ Physic-System ################ */
void System2::addVelocity(Entity& _entity, glm::vec3 _velocity) {
	registry.addComponent<Velocity>(_entity, _velocity);
}

void System2::setVelocity(Entity& _entity, glm::vec3 _velocity) {
	Velocity& velocity = registry.getComponentUnsafe<Velocity>(_entity);
	velocity.velocity = _velocity;
}

void System2::addAccelaration(Entity& _entity, glm::vec3 _velocity) {
	registry.addComponent<Accelaration>(_entity, _velocity);
}

void System2::setAccelaration(Entity& _entity, glm::vec3 _accelaration) {
	Accelaration& accelaration = registry.getComponentUnsafe<Accelaration>(_entity);
	accelaration.accelaration = _accelaration;
}

void System2::addMass(Entity& _entity, float _mass) {
	registry.addComponent<Mass>(_entity, _mass);
}

void System2::setMass(Entity& _entity, float _mass) {
	Mass& mass = registry.getComponentUnsafe<Mass>(_entity);
	mass.mass = _mass;
}

void System2::addAnchor(Entity& _entity, glm::vec3 _anchor) {
	registry.addComponent<Anchor>(_entity, _anchor);
}

void System2::setAnchor(Entity& _entity, glm::vec3 _anchor) {
	Anchor& anchor = registry.getComponentUnsafe<Anchor>(_entity);
	anchor.anchor = _anchor;
}

void System2::move(Entity& _entity, float _deltaTime) {
	glm::vec3 velocity = registry.getComponent<Velocity>(_entity)->velocity +
		registry.getComponent<Accelaration>(_entity)->accelaration * _deltaTime;
	setVelocity(_entity, velocity);
	transfromMultiply(_entity, glm::translate(velocity));
}

void System2::springY(Entity& _entity, float _deltaTime) {
	float k = 20.f;

	glm::mat4 transfrom = registry.getComponent<Transform>(_entity)->transform;
	float positionEntityY = transfrom[3][1];
	float positionAnchorY = registry.getComponent<Anchor>(_entity)->anchor[1];
	float springForceY = -k * (positionEntityY - positionAnchorY);

	float mass = registry.getComponent<Mass>(_entity)->mass;
	float gravity = registry.getComponent<Accelaration>(_entity)->accelaration[0];
	float forceY = springForceY + mass * gravity;

	float accelerationY = forceY / mass;
	float velocityY = registry.getComponent<Velocity>(_entity)->velocity[1];
	velocityY += accelerationY * _deltaTime;
	setVelocity(_entity, glm::vec3(0.f, velocityY, 0.f));
	positionEntityY += velocityY * _deltaTime;

	transfrom[3][1] = positionEntityY;
	setTransform(_entity, transfrom);
}
