#include "Mesh.h"
#include "OpenGL.h"
#include "Data.h"

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

	SetType(Resource::MeshResource);

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

void Mesh::Save(Data & data) const
{
	data.AddUInt("UID", GetUID());
	data.AddInt("Type", GetType());
	data.AddString("Assets_path", GetAssetsPath());
	data.AddString("Library_path", GetLibraryPath());
}

void Mesh::Load(Data & data)
{
	SetUID(data.GetUInt("UID"));
	SetType((Resource::ResourceType)data.GetInt("Type"));
	SetAssetsPath(data.GetString("Assets_path"));
	SetLibraryPath(data.GetString("Library_path"));
}

void Mesh::CreateMeta() const
{
}
