#pragma once

struct Velocity {
	glm::vec3 velocity;
};

struct Accelaration {
	glm::vec3 accelaration;
};

struct Mass {
	float mass;
};

struct Anchor {
	glm::vec3 anchor;
};

class PhysicSystem
{
public:
	void addVelocity(Entity& _entity, glm::vec3 _velocity) {
		EntitySystem::registry.addComponent<Velocity>(_entity, _velocity);
	}

	void setVelocity(Entity& _entity, glm::vec3 _velocity) {
		Velocity& velocity = EntitySystem::registry.getComponentUnsafe<Velocity>(_entity);
		velocity.velocity = _velocity;
	}

	void addAccelaration(Entity& _entity, glm::vec3 _velocity) {
		EntitySystem::registry.addComponent<Accelaration>(_entity, _velocity);
	}

	void setAccelaration(Entity& _entity, glm::vec3 _accelaration) {
		Accelaration& accelaration = EntitySystem::registry.getComponentUnsafe<Accelaration>(_entity);
		accelaration.accelaration = _accelaration;
	}

	void addMass(Entity& _entity, float _mass) {
		EntitySystem::registry.addComponent<Mass>(_entity, _mass);
	}

	void setMass(Entity& _entity, float _mass) {
		Mass& mass = EntitySystem::registry.getComponentUnsafe<Mass>(_entity);
		mass.mass = _mass;
	}

	void addAnchor(Entity& _entity, glm::vec3 _anchor) {
		EntitySystem::registry.addComponent<Anchor>(_entity, _anchor);
	}

	void setAnchor(Entity& _entity, glm::vec3 _anchor) {
		Anchor& anchor = EntitySystem::registry.getComponentUnsafe<Anchor>(_entity);
		anchor.anchor = _anchor;
	}

	void setTransform(Entity& _entity, glm::mat4 _transform) {
		Transform& transform = EntitySystem::registry.getComponentUnsafe<Transform>(_entity);
		transform.transform = _transform;
	}

	void transfromMultiply(Entity& _entity, glm::mat4 _transform) {
		Transform& transform = EntitySystem::registry.getComponentUnsafe<Transform>(_entity);
		transform.transform *= _transform;
	}

	void move(Entity& _entity, float _deltaTime) {
		glm::vec3 velocity = EntitySystem::registry.getComponent<Velocity>(_entity)->velocity + 
			EntitySystem::registry.getComponent<Accelaration>(_entity)->accelaration * _deltaTime;
		setVelocity(_entity, velocity);
		transfromMultiply(_entity, glm::translate(velocity));
	}

	void springY(Entity& _entity, float _deltaTime) {
		float k = 20.f;
		
		glm::mat4 transfrom = EntitySystem::registry.getComponent<Transform>(_entity)->transform;
		float positionEntityY = transfrom[3][1];
		float positionAnchorY = EntitySystem::registry.getComponent<Anchor>(_entity)->anchor[1];
		float springForceY = -k * (positionEntityY - positionAnchorY);

		float mass = EntitySystem::registry.getComponent<Mass>(_entity)->mass;
		float gravity = EntitySystem::registry.getComponent<Accelaration>(_entity)->accelaration[0];
		float forceY = springForceY + mass * gravity;

		float accelerationY = forceY / mass;
		float velocityY = EntitySystem::registry.getComponent<Velocity>(_entity)->velocity[1];
		velocityY += accelerationY * _deltaTime;
		setVelocity(_entity, glm::vec3(0.f, velocityY, 0.f));
		positionEntityY += velocityY * _deltaTime;

		transfrom[3][1] = positionEntityY;
		setTransform(_entity, transfrom);
	}

private:

};
