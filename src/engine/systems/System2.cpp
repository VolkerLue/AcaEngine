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
void System2::draw() {
	renderer.clear();
	registry.execute<Mesh, Texture, Transform>([&](
		const Mesh& mesh, const Texture texture, const Transform& transform) {
			renderer.draw(mesh.mesh, *texture.texture, transform.transform);
		});
	renderer.present(camera);
}

void System2::drawEntity(Entity& _entity, const graphics::Texture2D& _texture) {
	renderer.clear();
	renderer.draw(registry.getComponent<Mesh>(_entity)->mesh, _texture, registry.getComponent<Transform>(_entity)->transform);
	renderer.present(camera);
}

void System2::setCamera(float _fov, float _zNear, float zFar) {
	camera = graphics::Camera(_fov, _zNear, zFar);
}

//void System2::removeIntersecting() {
//	OctreeNode oN(AABB{ 3, -1000, 1000, -1000, 1000 - 1000, 1000 });
//	registry.execute<Entity, AABB>([&](Entity ent, AABB& aabb)
//		{
//			oN.insert(aabb, ent, registry);
//		});
//}


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

void System2::updateTransform(float _deltaTime) {
	executeVelocity(_deltaTime);
	executeRotation(_deltaTime);
}

void System2::executeVelocity(float _deltaTime) {
	registry.execute<Transform, Velocity>([&](Transform& transform, Velocity& velocity) {
		transform.transform *= glm::translate(velocity.velocity * _deltaTime); });
}

void System2::executeRotation(float _deltaTime) {
	registry.execute<Transform, Rotation>([&](Transform& transform, Rotation& rotation) {
		transform.transform = glm::rotate(transform.transform, rotation.angleInRadians * _deltaTime, rotation.axisOfRotation); });
}

void System2::removeEntityWhenNotInView(std::list<Entity>& entities){
	int entityId = whichEntityIsNotInView();
	if (entityId != -1) {
		Entity entity{ static_cast<uint32_t>(entityId) };
		eraseEntity(entity);
		entities.remove_if([&entity](const Entity& _entity) { return _entity.id == entity.id; });
	}
}

int System2::whichEntityIsNotInView() {
	int found = -1;
	registry.execute<Entity, Transform>([&found](Entity _entity, const Transform& transform) {
			glm::vec3 position = glm::vec3(transform.transform[3][0], transform.transform[3][1], transform.transform[3][2]);
			if (position[0] < (-75.f) || position[0] > (75.f) ||
				position[1] < (-50.f) || position[1] > (50.f) ||
				position[2] < (-100.f) || position[2] > (0.f))
			{ 
				found = _entity.id;
				return found;
			}});	
	return found;
}

void System2::shootSphere(std::list<Entity>& _entities, float _velocity, const graphics::Texture2D& _texture) {
	if (inputManager.isButtonPressed(input::MouseButton::LEFT)) {
		glm::vec3 curserPos = camera.toWorldSpace(inputManager.getCursorPos());
		Entity entity;
		_entities.push_back(createEntity(entity));
		addTexture(_entities.back(), &_texture);
		addMesh(_entities.back(), "models/sphere.obj");
		addTransform(_entities.back(), glm::translate(curserPos));		
		addVelocity(_entities.back(), glm::vec3(curserPos[0] * _velocity, curserPos[1] * _velocity, curserPos[2] * _velocity));
		//system.addAABB(entities.back(), 1);
	}
}

//void System2::rotate(Entity& _entity, float _deltatime) {
//	Transform& transform = registry.getComponentUnsafe<Transform>(_entity);
//	AngularVelocity& velo = registry.getComponentUnsafe<AngularVelocity>(_entity);
//	glm::quat Quat = glm::quat(glm::vec3(velo.angular_velocity.x * _deltatime, velo.angular_velocity.y * _deltatime, velo.angular_velocity.z * _deltatime));
//	glm::mat4 RotationMatrix = glm::toMat4(Quat);
//	transform.transform *= RotationMatrix;
//}

//void System2::updateAABB() {
//	registry.execute<Entity, AABB>([&](Entity _ent, AABB& _aabb)
//		{
//			AABB aabb;
//			_aabb = aabb.calculateAABB(registry.getComponent<Mesh>(_ent)->mesh,
//				registry.getComponent<Transform>(_ent)->transform, _aabb.type);
//		});
//}


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

void System2::addTexture(Entity& _entity, const graphics::Texture2D* _texture) {
	registry.addComponent<Texture>(_entity, _texture);
}

void System2::setTexture(Entity& _entity, const graphics::Texture2D* _texture) {
	auto texture = registry.getComponentUnsafe<Texture>(_entity);
	texture.texture = _texture;
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

//void System2::addRotation(Entity& _entity, glm::vec3 _eulerAngles) {
//	registry.addComponent<Rotation>(_entity, _eulerAngles);
//}

//void System2::addAngularVelocity(Entity& _entity, glm::vec3 _angular_velocity) {
//	registry.addComponent<AngularVelocity>(_entity, _angular_velocity);
//}

//void System2::addAABB(Entity& ent, int type) {
//	AABB aabb;
//	aabb = aabb.calculateAABB(registry.getComponent<Mesh>(ent)->mesh,
//		registry.getComponent<Transform>(ent)->transform, type);
//	registry.addComponent<AABB>(ent, type, aabb.minX, aabb.maxX, aabb.minY, aabb.maxY, aabb.minZ, aabb.maxZ);
//}


/* ################ Utils-System ################ */
int System2::randomWithoutZero(int quantity, int start) {
	int random = 0;
	while (random == 0) {
		random = rand() % quantity + (start);
	};
	return random;
}
