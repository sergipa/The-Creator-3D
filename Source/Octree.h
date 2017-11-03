#pragma once
#include "MathGeoLib\Geometry\AABB.h"
#include <list>

#define MAX_DIVISIONS 3

class OctreeNode
{
public:
	OctreeNode(AABB& box);
	~OctreeNode();

	bool NodeIsFull() const;
	void InsertInNode(AABB* node_content);
	void EraseInNode(AABB* bounding_box);
	void DivideNode();
	void ClearNode();
	void CollectIntersections(std::list<AABB*>& intersections_list, AABB* bounding_box);
	void DebugDrawNode();

public:
	AABB node_box;
	std::list<AABB*> node_contents;
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
	void Update();
	void Insert(AABB* bounding_box);
	void Erase(AABB* bounding_box);
	void CollectIntersections(std::list<AABB*>& intersections_list, AABB* bounding_box);
	void DebugDraw();
	void CalculateNewSize(float3 box_min_point, float3 box_max_point);
	void CheckIfNeedsUpdate(float3 box_min_point, float3 box_max_point, bool inserting_box);

private:
	OctreeNode* root_node = nullptr;
	float3 min_point;
	float3 max_point;
	
public:
	bool update_tree;
};

