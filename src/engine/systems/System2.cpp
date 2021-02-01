#include "System2.hpp"
#include <map>
#include <iostream>



System2::System2() : registry(),
camera(graphics::Camera(45.f, 1.f, 150.f)),
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
	registry.execute<Entity, Mesh, Texture, Transform>([&](
		Entity ent, const Mesh& mesh, const Texture texture, const Transform& transform) {
			renderer.clear();
			uploadLights(ent);
			renderer.draw(*mesh.mesh, *texture.texture, transform.transform);
			renderer.present(camera);
		});
	
}

void System2::drawEntity(Entity& _entity, const graphics::Texture2D& _texture) {
	renderer.clear();
	renderer.draw(*registry.getComponent<Mesh>(_entity)->mesh, _texture, registry.getComponent<Transform>(_entity)->transform);
	renderer.present(camera);
}

void System2::setCamera(float _fov, float _zNear, float zFar) {
	camera = graphics::Camera(_fov, _zNear, zFar);
}

void System2::uploadLights(Entity ent) {
	//get Constants and upload them
	float kc;
	float kq;
	float ke;
	registry.execute<LightConstants>([&](LightConstants& lc) {
		kc = lc.kc;
		kq = lc.kq;
		ke = lc.ke;
		});
	graphics::glCall(glUniform1f, renderer.program.getUniformLoc("kc"), kc);
	graphics::glCall(glUniform1f, renderer.program.getUniformLoc("kq"), kq);
	graphics::glCall(glUniform1f, renderer.program.getUniformLoc("ke"), ke);
	
	//find nearest lights and upload them
	Box& box = registry.getComponentUnsafe<Box>(ent);
	glm::vec3 min = box.transformedAabb.min;
	glm::vec3 max = box.transformedAabb.max;
	float distance;
	float intens;
	std::map<float, Entity, std::greater<float>> lights;
	registry.execute<Entity, PointLight>([&](Entity ent, PointLight& pl) {
		distance = std::min(glm::distance(min, pl.position), glm::distance(max, pl.position));
		if (distance < pl.AOE) {
			intens = pl.intensity / (1 + kc * distance + kq * distance * distance + glm::exp(-ke * distance));
			lights[intens] = ent;
		}
		});
	unsigned numLights = lights.size();
	const int maxLights = 8;
	if (numLights > maxLights) numLights = maxLights;
	float lightIntensity[maxLights];
	float lightColors[maxLights * 3];
	float lightPositions[maxLights * 3];
	int i = 0;
	for (auto it = lights.begin(); it != lights.end() && i < numLights; it++) {
		PointLight& pl = registry.getComponentUnsafe<PointLight>(it->second);
		lightIntensity[i] = pl.intensity;
		lightColors[i * 3] = pl.color.x;
		lightColors[i * 3 + 1] = pl.color.y;
		lightColors[i * 3 + 2] = pl.color.z;
		lightPositions[i * 3] = pl.position.x;
		lightPositions[i * 3 + 1] = pl.position.y;
		lightPositions[i * 3 + 2] = pl.position.z;
		i++;
	}
	graphics::glCall(glUniform1i, renderer.program.getUniformLoc("numPointsLights"), numLights);
	graphics::glCall(glUniform1fv, renderer.program.getUniformLoc("pointLightIntensity"), numLights, lightIntensity);
	graphics::glCall(glUniform3fv, renderer.program.getUniformLoc("pointLightColor"), numLights, lightColors);
	graphics::glCall(glUniform3fv, renderer.program.getUniformLoc("pointLightPos"), numLights, lightPositions);
}


/* ################ Physic-System ################ */
void System2::updateTransform(float _deltaTime) {

	registry.execute<Transform, Position>([&](Transform& _transform, const Position& _position)	{
			_transform.transform = glm::translate(glm::mat4(1.0f), _position.position);	
	});

	registry.execute<Transform, Orientation>([&](Transform& _transform, const Orientation& _orientation){							
		_transform.transform *= glm::toMat4(_orientation.orientation);
	});

	registry.execute<Transform, Scale>([&](Transform& _transform, const Scale& _scale){				
		_transform.transform *= glm::scale(glm::mat4(1.0f), _scale.scale);
	});
}

void System2::updatePosition(float _deltaTime) {

	registry.execute<Position, Velocity>([&](Position& _position, const Velocity& _velocity)
		{
			_position.position += _velocity.velocity *_deltaTime;
		}
	);
}

void System2::updateOrientation(float _deltaTime) {

	registry.execute<Orientation, AngularVelocity>([&](Orientation& _orientation, const AngularVelocity& _angularVelocity)
		{
			_orientation.orientation *= glm::quat(_angularVelocity.angularVelocity * glm::pi<float>() * _deltaTime);
		}
	);
}

void System2::updateAABB() {
	registry.execute<Box, Transform>([&](Box& box, Transform transform) {
		std::vector<glm::vec3> corners;
		corners.push_back(glm::vec3(box.aabb.min.x, box.aabb.min.y, box.aabb.min.z));
		corners.push_back(glm::vec3(box.aabb.min.x, box.aabb.min.y, box.aabb.max.z));
		corners.push_back(glm::vec3(box.aabb.min.x, box.aabb.max.y, box.aabb.min.z));
		corners.push_back(glm::vec3(box.aabb.min.x, box.aabb.max.y, box.aabb.max.z));
		corners.push_back(glm::vec3(box.aabb.max.x, box.aabb.min.y, box.aabb.min.z));
		corners.push_back(glm::vec3(box.aabb.max.x, box.aabb.min.y, box.aabb.max.z));
		corners.push_back(glm::vec3(box.aabb.max.x, box.aabb.max.y, box.aabb.min.z));
		corners.push_back(glm::vec3(box.aabb.max.x, box.aabb.max.y, box.aabb.max.z));
		glm::vec3 current;
		auto it = corners.begin();
		glm::vec3 min(glm::vec3(camera.getViewProjection() * transform.transform * glm::vec4(*it, 1)));
		glm::vec3 max(glm::vec3(camera.getViewProjection() * transform.transform * glm::vec4(*it, 1)));
		it++;
		for (; it != corners.end(); it++) {
			current = glm::vec3(camera.getViewProjection() * transform.transform * glm::vec4(*it, 1));
			if (current.x < min.x) min.x = current.x;
			if (current.x > max.x) max.x = current.x;
			if (current.y < min.y) min.y = current.y;
			if (current.y > max.y) max.y = current.y;
			if (current.z < min.z) min.z = current.z;
			if (current.z > max.z) max.z = current.z;
		}
		box.transformedAabb.max = max;
		box.transformedAabb.min = min;
		});
}

int System2::removeIntersecting() {
	utils::SparseOctree<Entity, 3, float> sparseOctree;
	registry.execute<Entity, Box>([&](Entity ent, Box box) {
		if (!box.isProjectile) sparseOctree.insert(box.transformedAabb, ent);
		});
	std::set<Entity> ent;
	registry.execute<Box>([&](Box box) {
		if (box.isProjectile) {
			utils::SparseOctree<Entity, 3, float>::AABBQuery aabbQuery(box.transformedAabb);
			sparseOctree.traverse(aabbQuery);
			for (auto it = aabbQuery.hits.begin(); it != aabbQuery.hits.end(); it++) {
				ent.insert(*it);
			}
		}
		});
	for (auto it = ent.begin(); it != ent.end(); it++) {
		registry.erase(*it);
	}

	return ent.size();
}

void System2::shootMeshWithTexure(const graphics::Mesh* _mesh, const graphics::Texture2D& _texture, std::list<Entity>& _entities, float _velocity) {
	if (inputManager.isButtonPressed(input::MouseButton::LEFT)) {
		glm::vec3 curserPos = camera.toWorldSpace(inputManager.getCursorPos());
		Entity entity;
		_entities.push_back(createEntity(entity));
		addTexture(_entities.back(), &_texture);
		addMesh(_entities.back(), _mesh);
		addTransform(_entities.back(), glm::mat4(1.f));
		addPosition(_entities.back(), glm::vec3(curserPos));
		addScale(_entities.back(), glm::vec3(0.7f, 0.7f, 0.7f));
		addVelocity(_entities.back(), glm::vec3(curserPos * _velocity));
		addAABB(_entities.back(), true);
	}
}

void System2::move(Entity& _entity, float _deltaTime) {
	glm::vec3 velocity = registry.getComponent<Velocity>(_entity)->velocity +
		registry.getComponent<Accelaration>(_entity)->accelaration * _deltaTime;
	setVelocity(_entity, velocity);
	Transform& transform = registry.getComponentUnsafe<Transform>(_entity);
	transform.transform *= glm::translate(velocity);
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


/* ################ Component-System ################ */
void System2::addMesh(Entity& _entity, const graphics::Mesh* _mesh) {
	registry.addComponent<Mesh>(_entity, _mesh);
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
	auto& texture = registry.getComponentUnsafe<Texture>(_entity);
	texture.texture = _texture;
}

void System2::addPosition(Entity& _entity, glm::vec3 _position) {
	registry.addComponent<Position>(_entity, _position);
}

void System2::setPosition(Entity& _entity, glm::vec3 _position) {
	Position& position = registry.getComponentUnsafe<Position>(_entity);
	position.position = _position;
}

void System2::addOrientation(Entity& _entity, glm::quat _orientation) {
	registry.addComponent<Orientation>(_entity, _orientation);
}

void System2::setOrientation(Entity& _entity, glm::quat _orientation) {
	Orientation& orientation = registry.getComponentUnsafe<Orientation>(_entity);
	orientation.orientation = _orientation;
}

void System2::addScale(Entity& _entity, glm::vec3 _scale) {
	registry.addComponent<Scale>(_entity, _scale);
}

void System2::setScale(Entity& _entity, glm::vec3 _scale) {
	Scale& scale = registry.getComponentUnsafe<Scale>(_entity);
	scale.scale = _scale;
}

void System2::addVelocity(Entity& _entity, glm::vec3 _velocity) {
	registry.addComponent<Velocity>(_entity, _velocity);
}

void System2::setVelocity(Entity& _entity, glm::vec3 _velocity) {
	Velocity& velocity = registry.getComponentUnsafe<Velocity>(_entity);
	velocity.velocity = _velocity;
}

void System2::addAngularVelocity(Entity& _entity, glm::vec3 _angularVelocity) {
	registry.addComponent<AngularVelocity>(_entity, _angularVelocity);
}

void System2::setAngularVelocity(Entity& _entity, glm::vec3 _angularVelocity) {
	AngularVelocity& angularVelocity = registry.getComponentUnsafe<AngularVelocity>(_entity);
	angularVelocity.angularVelocity = _angularVelocity;
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

void System2::addAABB(Entity& ent, bool isProjectile) {
	const graphics::Mesh mesh = *(registry.getComponent<Mesh>(ent)->mesh);
	glm::mat4 transform = camera.getViewProjection() * registry.getComponent<Transform>(ent)->transform;
	auto it = mesh.vertices.begin();
	glm::vec3 min(it->Position);
	glm::vec3 max(it->Position);
	glm::vec3 minTransformed(glm::vec3(transform * glm::vec4(it->Position, 1)));
	glm::vec3 maxTransformed(glm::vec3(transform * glm::vec4(it->Position, 1)));
	it++;
	glm::vec3 current;
	glm::vec3 currentTransformed;
	for (; it != mesh.vertices.end(); it++) {
		currentTransformed = glm::vec3(transform * glm::vec4(it->Position, 1));
		if (currentTransformed.x < minTransformed.x) minTransformed.x = currentTransformed.x;
		if (currentTransformed.x > maxTransformed.x) maxTransformed.x = currentTransformed.x;
		if (currentTransformed.y < minTransformed.y) minTransformed.y = currentTransformed.y;
		if (currentTransformed.y > maxTransformed.y) maxTransformed.y = currentTransformed.y;
		if (currentTransformed.z < minTransformed.z) minTransformed.z = currentTransformed.z;
		if (currentTransformed.z > maxTransformed.z) maxTransformed.z = currentTransformed.z;
		current = it->Position;
		if (current.x < min.x) min.x = current.x;
		if (current.x > max.x) max.x = current.x;
		if (current.y < min.y) min.y = current.y;
		if (current.y > max.y) max.y = current.y;
		if (current.z < min.z) min.z = current.z;
		if (current.z > max.z) max.z = current.z;
	}
	registry.addComponent<Box>(ent, isProjectile, math::AABB<3>(min, max), math::AABB<3>(minTransformed, maxTransformed));
}

void System2::addPointLight(Entity& ent, glm::vec3 position, glm::vec3 color, float intensity)
{
	float kc;
	float kq;
	float ke;
	registry.execute<LightConstants>([&](LightConstants& lc) {
		kc = lc.kc;
		kq = lc.kq;
		ke = lc.ke;
		});
	float intens = 0.0f;
	int d = 0;
	for (; d < 1000; d++) {
		intens = intensity / (1 + kc * d + kq * d * d + glm::exp(-ke * d));
		if (intens < 0.01) {
			d = d - 1;
			break;
		}
	}
	registry.addComponent<PointLight>(ent, position, color, intensity, d);
}

void System2::addLightConstants(float kc, float kq, float ke)
{
	registry.addComponent<LightConstants>(registry.create(), kc, kq, ke);
}

void System2::setLightConstants(float kc, float kq, float ke)
{
	registry.execute<LightConstants>([&](LightConstants& lc) {
		lc.kc = kc;
		lc.kq = kq;
		lc.ke = ke;
		});
}


/* ################ Utils-System ################ */
int System2::randomWithoutZero(int quantity, int start) {
	int random = 0;
	while (random == 0) {
		random = rand() % quantity + (start);
	};
	return random;
}

int System2::whichEntityIsNotInView() {
	int found = -1;
	registry.execute<Entity, Transform>([&found](Entity _entity, const Transform& transform) {
		glm::vec3 position = glm::vec3(transform.transform[3][0], transform.transform[3][1], transform.transform[3][2]);
		if (position[0] < (-76.f) || position[0] > (76.f) ||
			position[1] < (-51.f) || position[1] > (51.f) ||
			position[2] < (-150.f) || position[2] > (0.f))
		{
			found = _entity.id;
			return found;
		}});
	return found;
}
