#include "Mesh.h"


Mesh::Mesh()
{
	id_vertices = 0;
	num_indices = 0;
	indices = nullptr;

	id_indices = 0;
	num_vertices = 0;
	vertices = nullptr;
}

Mesh::~Mesh()
{
}
