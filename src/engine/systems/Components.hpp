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