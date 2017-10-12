#pragma once
#include "Module.h"
#include "Assimp/include/cimport.h"
#include <string>

class GameObject;

class ModuleImport :
	public Module
{
public:
	ModuleImport(Application * app, bool start_enabled = true);
	~ModuleImport();

	bool CleanUp();

	void LoadFile(std::string path);
	bool LoadMesh(const char* path);
	int LoadTexture(const char* path);

	std::string GetFileName(const char* path);

private:
	aiLogStream stream;
	GameObject* parent;
};

