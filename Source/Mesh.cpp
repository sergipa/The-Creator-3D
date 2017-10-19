#include "Mesh.h"
#include "OpenGL.h"

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
	RELEASE_ARRAY(indices);
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(normals);
	RELEASE_ARRAY(colors);
	RELEASE_ARRAY(texture_coords);

	glDeleteBuffers(1, &id_vertices);
	glDeleteBuffers(1, &id_indices);
	glDeleteBuffers(1, &id_normals);
	glDeleteBuffers(1, &id_colors);
	glDeleteBuffers(1, &id_texture_coords);

	id_vertices = 0;
	id_indices = 0;
	id_normals = 0;
	id_colors = 0;
	id_texture_coords = 0;
}
