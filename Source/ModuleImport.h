#pragma once
#include "Module.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Mesh.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

class ModuleImport :
	public Module
{
public:
	ModuleImport(Application * app, bool start_enabled = true);
	~ModuleImport();

	bool CleanUp();

	bool LoadMesh(const char* path);

private:
	struct aiLogStream stream;
	Mesh* mesh;
};

