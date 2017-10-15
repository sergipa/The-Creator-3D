#pragma once
#include "Module.h"
#include <list>

class Texture;
class Mesh;

class ModuleResources :
	public Module
{
public:
	ModuleResources(Application* app, bool start_enabled = true);
	~ModuleResources();

	bool TextureExist(std::string name) const;
	Texture* GetTexture(std::string name) const;
	void AddTexture(Texture* texture);

	bool MeshExist(std::string name) const;
	Mesh* GetMesh(std::string name) const;
	void AddMesh(Mesh* mesh);

private:
	std::list<Texture*> textures_list;
	std::list<Mesh*> meshes_list;
};

