#pragma once

#include <string>
#include "MathGeoLib\Geometry\GeometryAll.h"
#include "Resource.h"

class Data;

class Mesh : public Resource
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

	AABB box;

	void Save(Data& data) const;
	void Load(Data& data);
	void CreateMeta() const;
};

