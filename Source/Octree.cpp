#include "Octree.h"
#include "Globals.h"
#include "Primitive.h"

OctreeNode::OctreeNode(AABB& box)
{
	root_parent_node = node_childs[0] = node_childs[1] = node_childs[2] = node_childs[3] = 
				  node_childs[4] = node_childs[5] = node_childs[6] = node_childs[7] = nullptr;
	node_box = box;
}

OctreeNode::~OctreeNode()
{
	RELEASE(root_parent_node);
	for (int i = 0; i < 8; ++i)
	{
		RELEASE(node_childs[i]);
	}
}

bool OctreeNode::NodeIsLeaf() const
{
	return node_childs[0] == nullptr;
}

bool OctreeNode::NodeIsFull() const
{
	return node_contents.size() == 8;
}

void OctreeNode::InsertInNode(AABB * node_content)
{
	if (NodeIsLeaf()/* && root_parent_node->division_level < MAX_DIVISIONS*/)
	{
		if (!NodeIsFull())
		{
			node_contents.push_back(node_content);
		}
		else
		{
			DivideNode();
		}
	}
	
	if (node_childs[0] != nullptr)
	{
		for (int i = 0; i < 8; i++)
		{
			if (node_childs[i]->node_box.Intersects(*node_content))
			{
				node_childs[i]->InsertInNode(node_content);
				break;
			}
		}
	}
}

void OctreeNode::DivideNode()
{
	AABB child_box;
	float3 childs_side_length = node_box.Size() / 2;
	float3 center = node_box.CenterPoint();

	int child_index = 0;
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			for (int z = 0; z < 2; z++)
			{
				float3 min_child_point(
					node_box.minPoint.x + z * childs_side_length.x,
					node_box.minPoint.y + y * childs_side_length.y, 
					node_box.minPoint.z + x * childs_side_length.z
				);

				float3 max_child_point(
					min_child_point.x + childs_side_length.x,
					min_child_point.y + childs_side_length.y,
					min_child_point.z + childs_side_length.z
				);
				child_box.minPoint = min_child_point;
				child_box.maxPoint = max_child_point;
				node_childs[child_index] = new OctreeNode(child_box);
				//node_childs[child_index]->root_parent_node = root_parent_node;
				child_index++;
			}
		}
	}
	//root_parent_node->division_level++;
}

void OctreeNode::ClearNode()
{
	RELEASE(node_childs[0]);
}

void OctreeNode::CollectIntersections(std::list<AABB*>& intersections_list, AABB * bounding_box)
{
	if (node_childs[0] != nullptr)
	{
		for (int i = 0; i < 8; i++)
		{
			if (node_childs[i]->node_box.Intersects(*bounding_box))
			{
				node_childs[i]->CollectIntersections(intersections_list, bounding_box);
			}
		}
	}

	for (std::list<AABB*>::iterator it = node_contents.begin(); it != node_contents.end(); it++)
	{
		if ((*it)->Intersects(*bounding_box))
		{
			intersections_list.push_back(*it);
		}
	}
}

void OctreeNode::DebugDrawNode()
{
	if (node_childs[0] != nullptr)
	{
		for (int i = 0; i < 8; i++)
		{
			node_childs[i]->DebugDrawNode();
		}
	}
	DebugAABB aabb(node_box);
	aabb.Render();
}

//////////////////////////////////////////////

Octree::Octree()
{
}

Octree::~Octree()
{
	Clear();
}

void Octree::Create(float3 min_point, float3 max_point)
{
	Clear();
	AABB box(min_point, max_point);
	root_node = new OctreeNode(box);
	//root_node->root_parent_node = root_node;
}

void Octree::Clear()
{
	RELEASE(root_node);
}

void Octree::Insert(AABB * bounding_box)
{
	if (root_node != nullptr)
	{
		if (bounding_box->Intersects(root_node->node_box))
		{
			root_node->InsertInNode(bounding_box);
		}
	}	
}

void Octree::CollectIntersections(std::list<AABB*>& intersections_list, AABB* bounding_box)
{
	if (root_node != nullptr)
	{
		if (bounding_box->Intersects(root_node->node_box))
		{
			root_node->CollectIntersections(intersections_list, bounding_box);
		}
	}
}

void Octree::DebugDraw()
{
	if (root_node != nullptr)
	{
		root_node->DebugDrawNode();
	}
}
