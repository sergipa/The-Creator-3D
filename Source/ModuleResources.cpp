#include "ModuleResources.h"
#include "Texture.h"
#include "Mesh.h"

ModuleResources::ModuleResources(Application* app, bool start_enabled, bool is_game) : Module(app, start_enabled, is_game)
{
	name = "Resources";
}

ModuleResources::~ModuleResources()
{
	for (std::list<Texture*>::iterator it = textures_list.begin(); it != textures_list.end(); ++it) {
		RELEASE(*it);
	}
	for (std::list<Mesh*>::iterator it = meshes_list.begin(); it != meshes_list.end(); ++it) {
		RELEASE(*it);
	}
}

bool ModuleResources::TextureExist(std::string name) const
{
	for (std::list<Texture*>::const_iterator it = textures_list.begin(); it != textures_list.end(); it++)
	{
		if ((*it) != nullptr && (*it)->GetName() == name) return true;
	}
	return false;
}

Texture * ModuleResources::GetTexture(std::string name) const
{
	for (std::list<Texture*>::const_iterator it = textures_list.begin(); it != textures_list.end(); it++)
	{
		if ((*it) != nullptr && (*it)->GetName() == name) return (*it);
	}
	return nullptr;
}

void ModuleResources::AddTexture(Texture * texture)
{
	if (texture != nullptr) textures_list.push_back(texture);
}

bool ModuleResources::MeshExist(std::string name) const
{
	for (std::list<Mesh*>::const_iterator it = meshes_list.begin(); it != meshes_list.end(); it++)
	{
		if ((*it) != nullptr && (*it)->GetName() == name) return true;
	}
	return false;
}

Mesh * ModuleResources::GetMesh(std::string name) const
{
	for (std::list<Mesh*>::const_iterator it = meshes_list.begin(); it != meshes_list.end(); it++)
	{
		if ((*it) != nullptr && (*it)->GetName() == name) return (*it);
	}
	return nullptr;
}

void ModuleResources::AddMesh(Mesh * mesh)
{
	if (mesh != nullptr) meshes_list.push_back(mesh);
}
