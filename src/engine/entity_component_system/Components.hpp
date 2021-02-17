#include "../graphics/renderer/meshrenderer.hpp"
#include "../math/geometrictypes.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

struct Mesh {
	const graphics::Mesh* mesh;
};

struct Transform {
	glm::mat4 transform;
};

struct Texture {
	const graphics::Texture2D* texture;
};

struct Position {
	glm::vec3 position;
};

struct Orientation {
	glm::quat orientation;
};

struct Scale {
	glm::vec3 scale;
};

struct Velocity {
	glm::vec3 velocity;
};

struct AngularVelocity {
	glm::vec3 angularVelocity;
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

struct PointLight {
	glm::vec3 position;
	glm::vec3 color;
	float intensity;
	int AOE;
};

struct LightConstants {
	float kc;
	float kq;
	float ke;
};
