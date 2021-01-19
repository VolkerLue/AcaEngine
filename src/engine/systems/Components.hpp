#pragma once
#include "../graphics/renderer/meshrenderer.hpp"

struct Mesh {
	graphics::Mesh mesh;
};

struct Transform {
	glm::mat4 transform;
};

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

struct Rotation {
	float angleInRadians;
	glm::vec3 axisOfRotation;
};

struct CursorPosition {
	glm::vec3 curserPosition;
};

struct Alive {
	bool alive;
};

/*
struct Rotation {
	glm::vec3 EulerAngles;
};

struct AngularVelocity {
	glm::vec3 angular_velocity;
};

*\
