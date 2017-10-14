#pragma once

typedef unsigned int uint;

#include <string>
#include "MathGeoLib\Geometry\GeometryAll.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	uint id_indices; // id in VRAM
	uint num_indices;
	uint* indices;

	uint id_vertices; // id in VRAM
	uint num_vertices;
	float* vertices;

	uint id_normals;
	float* normals;

	uint id_colors;
	float* colors;

	uint id_texture_coords;
	float* texture_coords;

	std::string name;

	AABB box;
};

