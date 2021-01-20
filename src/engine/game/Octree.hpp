#pragma once
#include <glm/glm.hpp>
#include <limits>
#include "../systems/Components.hpp"
#include "Registry2.hpp"

struct AABB {
	int type;	//1 = projectile, 2 = target, 3 = Octreebox
	double minX;
	double maxX;
	double minY;
	double maxY;
	double minZ;
	double maxZ;

	AABB calculateAABB(const graphics::Mesh& mesh, const glm::mat4 transform, int type) {
		glm::vec3 current;
		double minX = std::numeric_limits<double>::max();
		double maxX = std::numeric_limits<double>::min();
		double minY = std::numeric_limits<double>::max();
		double maxY = std::numeric_limits<double>::min();
		double minZ = std::numeric_limits<double>::max();
		double maxZ = std::numeric_limits<double>::min();
		for (auto it = mesh.vertices.begin(); it != mesh.vertices.end(); it++) {
			current = glm::vec3(transform * glm::vec4(it->Position, 1));
			if (current.x < minX) minX = current.x;
			if (current.x > maxX) maxX = current.x;
			if (current.y < minY) minY = current.y;
			if (current.y > maxY) maxY = current.x;
			if (current.z < minZ) minZ = current.z;
			if (current.z > maxZ) maxZ = current.z;
		}
		return AABB{ type, minX, maxX, minY, maxY, minZ, maxZ };
	}
	bool intersect(AABB& a, AABB& b) {
		return (a.minX <= b.maxX && a.maxX >= b.minX &&
			a.minY <= b.maxY && a.maxY >= b.minY &&
			a.minZ <= b.maxZ && a.maxZ >= b.minZ);
	}

	bool contains(AABB& container, AABB& content) {
		return (container.minX <= content.minX && container.maxX >= content.maxX &&
			container.minY <= content.minY && container.maxY >= content.maxY &&
			container.minZ <= content.minZ && container.maxZ >= content.maxZ);
	}

	AABB recalculateAABB(AABB aabb, const glm::mat4 transform) {
		std::vector<glm::vec3> corners;
		corners.push_back(glm::vec3(aabb.minX, aabb.minY, aabb.minZ));
		corners.push_back(glm::vec3(aabb.minX, aabb.minY, aabb.maxZ));
		corners.push_back(glm::vec3(aabb.minX, aabb.maxY, aabb.minZ));
		corners.push_back(glm::vec3(aabb.minX, aabb.maxY, aabb.maxZ));
		corners.push_back(glm::vec3(aabb.maxX, aabb.minY, aabb.minZ));
		corners.push_back(glm::vec3(aabb.maxX, aabb.minY, aabb.maxZ));
		corners.push_back(glm::vec3(aabb.maxX, aabb.maxY, aabb.minZ));
		corners.push_back(glm::vec3(aabb.maxX, aabb.maxY, aabb.maxZ));
		glm::vec3 current;
		double minX = std::numeric_limits<double>::max();
		double maxX = std::numeric_limits<double>::min();
		double minY = std::numeric_limits<double>::max();
		double maxY = std::numeric_limits<double>::min();
		double minZ = std::numeric_limits<double>::max();
		double maxZ = std::numeric_limits<double>::min();
		for (auto it = corners.begin(); it != corners.end(); it++) {
			current = glm::vec3(transform * glm::vec4(*it, 1));
			if (current.x < minX) minX = current.x;
			if (current.x > maxX) maxX = current.x;
			if (current.y < minY) minY = current.y;
			if (current.y > maxY) maxY = current.x;
			if (current.z < minZ) minZ = current.z;
			if (current.z > maxZ) maxZ = current.z;
		}
		return AABB{ aabb.type, minX, maxX, minY, maxY, minZ, maxZ };
	}
};




class OctreeNode {
public:
	OctreeNode(AABB box);
	OctreeNode(OctreeNode* parent, AABB box);
	~OctreeNode();
	void insert(AABB& other, Entity ent, Registry2& registry);
	
	bool isRoot;
	bool hasChildren;
	bool hasContent;
	Entity ent;
	AABB* content;
	AABB box;
	std::vector<OctreeNode*> children;
	OctreeNode* parent;
};
