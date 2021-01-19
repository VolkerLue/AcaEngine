#include "Octree.hpp"

OctreeNode::OctreeNode(AABB box) {
	this->box = box;
	parent = nullptr;
	isRoot = true;
	hasChildren = false;
	hasContent = false;
	ent = Entity{ -1 };
}

OctreeNode::OctreeNode(OctreeNode* parent, AABB box) {
	this->parent = parent;
	this->box = box;
	isRoot = false;
	hasChildren = false;
	hasContent = false;
	ent = Entity{ -1 };
}

OctreeNode::~OctreeNode()
{
	if (hasChildren) {
		for (int i = 0; i < 8; i++) {
			delete children[i];
		}
	}
}


void OctreeNode::insert(const AABB& other, Entity otherEnt, Registry2& registry) {
	if (!contains(this->box, other)) return;
	if (hasContent) {
		if (this->content->type == 2 && other->type == 1) {
			Alive alive = registry.getComponent<Alive>(this->ent);
			alive.alive = false;
		} else if (this->content->type == 1 && other->type == 2) {
			Alive alive = registry.getComponent<Alive>(otherEnt);
			alive.alive = false;
		}
		return;
	}
	glm::vec3 center((this->box.maxX - this->box.minX) / 2 + this->box.minX,
		(this->box.maxY - this->box.minY) / 2 + this->box.minY,
		(this->box.maxZ - this->box.minZ) / 2 + this->box.minZ);
	int c = 0;
	bool insertedChildren = false;
	if (!hasChildren) {
		children.push_back(new OctreeNode(this, AABB(3, center.x, this->box.maxX, center.y, this->box.maxY, center.z, this->box.maxZ)));
		children.push_back(new OctreeNode(this, AABB(3, center.x, this->box.maxX, center.y, this->box.maxY, this->box.minZ, center.z)));
		children.push_back(new OctreeNode(this, AABB(3, center.x, this->box.maxX, this->box.minY, center.y, center.z, this->box.maxZ)));
		children.push_back(new OctreeNode(this, AABB(3, center.x, this->box.maxX, this->box.minY, center.y, this->box.minZ, center.z)));
		children.push_back(new OctreeNode(this, AABB(3, this->box.minX, center.x, center.y, this->box.maxY, center.z, this->box.maxZ)));
		children.push_back(new OctreeNode(this, AABB(3, this->box.minX, center.x, center.y, this->box.maxY, this->box.minZ, center.z)));
		children.push_back(new OctreeNode(this, AABB(3, this->box.minX, center.x, this->box.minY, center.y, center.z, this->box.maxZ)));
		children.push_back(new OctreeNode(this, AABB(3, this->box.minX, center.x, this->box.minY, center.y, this->box.minZ, center.z)));
		hasChildren = true;
		insertedChildren = true;
	}
	for (int i = 0; i < 8; i++) {
		if (contains(children[i], other)) return children[i].insert(other);
	}
	if (insertedChildren) {
		for (int i = 0; i < 8; i++) {
			delete children[i];
		}
		children.clear();
		hasChildren = false;
	}
	content = other;
	this->ent = otherEnt;
	hasContent = true;
}