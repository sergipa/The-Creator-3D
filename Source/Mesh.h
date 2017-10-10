#pragma once

typedef unsigned int uint;

class Mesh
{
public:
	Mesh();
	~Mesh();

	uint id_vertices; // id in VRAM
	uint num_indices;
	uint* indices;

	uint id_indices; // id in VRAM
	uint num_vertices;
	float* vertices;
};

