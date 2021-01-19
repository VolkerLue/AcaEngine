#include "System2.hpp"

System2::System2() : registry(),
camera(graphics::Camera(45.f, 0.1f, 100.f)),
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
void System2::drawEntity(Entity& _entity, const graphics::Texture2D& _texture) {
	renderer.clear();
	renderer.draw(registry.getComponent<Mesh>(_entity)->mesh, _texture, registry.getComponent<Transform>(_entity)->transform);
	renderer.present(camera);
}

void System2::setCamera(float _fov, float _zNear, float zFar) {
	camera = graphics::Camera(_fov, _zNear, zFar);
}

void System2::repositionCrate() {
	registry.execute<Transform, Rotation>([&](Transform& transform, const Rotation& rotation) {
		glm::vec3 position = glm::normalize(glm::vec3(transform.transform[3][0], transform.transform[3][1], transform.transform[3][2]));
		if (position[0] < (-1) || position[0] > (1) ||
			position[1] < (-1) || position[1] > (1) ||
			position[2] < (-1) || position[2] > (0))
		{
			transform.transform = glm::translate(glm::vec3(0.f, 0.f, float(rand() % 30 + (-90))));
		}});
};

void System2::removeIntersecting() {
	OctreeNode oN(AABB{ 3, -100, 100, -100, 100, -100, 100 });
	EntitySystem::registry.execute<Entity, AABB>([&](Entity ent, AABB& aabb)
		{
			oN.insert(aabb, ent, EntitySystem::registry);
		});
}

/* ################ Physic-System ################ */
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

void System2::transfromMultiply(Entity& _entity, glm::mat4 _transform) {
	Transform& transform = registry.getComponentUnsafe<Transform>(_entity);
	transform.transform *= _transform;
}

void System2::updateTransformCrate(float _deltaTime) {
	registry.execute<Transform, Velocity, Rotation>([&](
		Transform& transform, const Velocity& velocity, const Rotation& rotation) {
			transform.transform *= glm::translate(velocity.velocity * _deltaTime),
				transform.transform = glm::rotate(transform.transform, rotation.angleInRadians * _deltaTime, rotation.axisOfRotation);
		});
}

void System2::updateTransformPlanet(float _deltaTime) {
	registry.execute<Transform, Velocity>([&](
		Transform& transform, const Velocity& velocity) {
			transform.transform *= glm::translate(velocity.velocity * _deltaTime);
		});
}

void System2::updateShoot(std::vector<Entity> entities) {

	registry.execute<Transform, Alive, CursorPosition>([&](Transform& transform, Alive& alive, const CursorPosition& cursorPosition) {
		glm::vec3 position = glm::normalize(glm::vec3(transform.transform[3][0], transform.transform[3][1], transform.transform[3][2]));
		if (position[0] < (-1) || position[0] > (1) ||
			position[1] < (-1) || position[1] > (1) ||
			position[2] < (-1) || position[2] > (0))
		{
			alive.alive = false;
		}});

	int i;
	if (inputManager.isButtonPressed(input::MouseButton::LEFT)) {
		glm::vec3 curserPosition = camera.toWorldSpace(inputManager.getCursorPos());		
		for (i = 100; i < 200; i++) {
			if (registry.getComponentUnsafe<Alive>(entities[i]).alive == false){ break; }	
			if (i == 199) { i++; }
		}
		if (i < 200) {
			setAlive(entities[i], true);
			setTransform(entities[i], glm::translate(curserPosition));
			setVelocity(entities[i], glm::vec3(curserPosition[0] * 500, curserPosition[1] * 500, curserPosition[2] * 500));
		}
	}
}

void System2::updateAABB() {
	EntitySystem::registry.execute<Entity, AABB>([&](Entity _ent, AABB& _aabb)
		{
			_aabb = _aabb.calculateAABB(EntitySystem::registry.getComponent<Mesh>(_ent)->mesh,
				EntitySystem::registry.getComponent<Transform>(_ent)->transform, _aabb.type);
		});
}


/* ################ Component-System ################ */
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

void System2::addRotation(Entity& _entity, float _angleInRadians, glm::vec3 _axisOfRotation) {
	registry.addComponent<Rotation>(_entity, _angleInRadians, _axisOfRotation);
}

void System2::setRotation(Entity& _entity, float _angleInRadians, glm::vec3 _axisOfRotation) {
	Rotation& rotation = registry.getComponentUnsafe<Rotation>(_entity);
	rotation.angleInRadians = _angleInRadians;
	rotation.axisOfRotation = _axisOfRotation;
}

void System2::addCursorPosition(Entity& _entity, glm::vec3 _curserPosition) {
	registry.addComponent<CursorPosition>(_entity, _curserPosition);
}

void System2::setCursorPosition(Entity& _entity, glm::vec3 _curserPosition) {
	CursorPosition& curserPositon = registry.getComponentUnsafe<CursorPosition>(_entity);
	curserPositon.curserPosition = _curserPosition;
}

void System2::addAlive(Entity& _entity, bool _alive) {
	registry.addComponent<Alive>(_entity, _alive);
}

void System2::setAlive(Entity& _entity, bool _alive) {
	Alive& alive = registry.getComponentUnsafe<Alive>(_entity);
	alive.alive = _alive;
}

void System2::addAABB(Entity& ent, int type) {
	AABB aabb;
	aabb = aabb.calculateAABB(EntitySystem::registry.getComponent<Mesh>(ent)->mesh,
		EntitySystem::registry.getComponent<Transform>(ent)->transform, type);
	EntitySystem::registry.addComponent<AABB>(ent, type, aabb.minX, aabb.maxX, aabb.minY, aabb.maxY, aabb.minZ, aabb.maxZ);
}


/* ################ Utils-System ################ */
int System2::randomWithoutZero(int quantity, int start) {
	int random = 0;
	while (random == 0) {
		random = rand() % quantity + (start);
	};
	return random;
}
