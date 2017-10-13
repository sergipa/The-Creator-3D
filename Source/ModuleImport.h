#pragma once
#include "Module.h"
#include "Assimp/include/cimport.h"
#include <string>

class GameObject;
class Texture;

class ModuleImport :
	public Module
{
public:
	ModuleImport(Application * app, bool start_enabled = true);
	~ModuleImport();

	bool CleanUp();

	void LoadFile(std::string path);
	bool LoadMesh(const char* path);
	Texture* LoadTexture(const char* path, bool attach_to_gameobject);

	std::string GetFileName(const char* path);

private:
	aiLogStream stream;
	GameObject* parent;
};

