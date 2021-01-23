#include "../graphics/renderer/meshrenderer.hpp"


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

//struct Rotation {
//	glm::vec3 EulerAngles;
//};

//struct AngularVelocity {
//	glm::vec3 angular_velocity;
//};

//struct AABB {
//	int type;	//1 = projectile, 2 = target, 3 = Octreebox
//	double minX;
//	double maxX;
//	double minY;
//	double maxY;
//	double minZ;
//	double maxZ;
//};
