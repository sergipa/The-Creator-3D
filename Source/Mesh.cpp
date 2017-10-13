#include "Mesh.h"


Mesh::Mesh()
{
	id_vertices = 0;
	num_indices = 0;
	indices = nullptr;

	id_indices = 0;
	num_vertices = 0;
	vertices = nullptr;

	id_normals = 0;
	normals = nullptr;

	id_colors = 0;
	colors = nullptr;

	id_texture_coords = 0;
	texture_coords = nullptr;
}

Mesh::~Mesh()
{
	delete indices;
	delete vertices;
	delete normals;
	delete colors;
	delete texture_coords;
}
