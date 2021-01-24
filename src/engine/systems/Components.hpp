#include "../graphics/renderer/meshrenderer.hpp"
#include "../math/geometrictypes.hpp"


struct Mesh {
	graphics::Mesh mesh;
};

struct Transform {
	glm::mat4 transform;
};

struct Texture {
	const graphics::Texture2D* texture;
};

struct Velocity {
	glm::vec3 velocity;
};

struct Rotation {
	float angleInRadians;
	glm::vec3 axisOfRotation;
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

struct Box {
	bool isProjectile;
	math::AABB<3> aabb;
	math::AABB<3> transformedAabb;
};

//struct Rotation {
//	glm::vec3 EulerAngles;
//};

//struct AngularVelocity {
//	glm::vec3 angular_velocity;
//};
