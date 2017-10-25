#pragma once
#include "MathGeoLib\Geometry\AABB.h"
#include <list>

#define MAX_DIVISIONS 3

class OctreeNode
{
public:
	OctreeNode(AABB& box);
	~OctreeNode();

	bool NodeIsLeaf() const;
	bool NodeIsFull() const;
	void InsertInNode(AABB* node_content);
	void DivideNode();
	void ClearNode();
	void CollectIntersections(std::list<AABB*>& intersections_list, AABB* bounding_box);
	void DebugDrawNode();

public:
	AABB node_box;
	std::list<AABB*> node_contents;
	OctreeNode* root_parent_node;
	OctreeNode* node_childs[8];
	int division_level = 0;
};

class Octree
{
public:
	Octree();
	~Octree();
	void Create(float3 min_point, float3 max_point);
	void Clear();
	void Insert(AABB* bounding_box);
	void CollectIntersections(std::list<AABB*>& intersections_list, AABB* bounding_box);
	void DebugDraw();

private:
	OctreeNode* root_node = nullptr;
};

