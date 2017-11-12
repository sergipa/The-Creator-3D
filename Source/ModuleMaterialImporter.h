#pragma once
#include "Module.h"

class Material;

class ModuleMaterialImporter :
	public Module
{
public:
	ModuleMaterialImporter(Application* app, bool start_enabled = true, bool is_game = false);
	~ModuleMaterialImporter();

	//Retuns the library path if created or an empty string
	std::string ImportMaterial(std::string path);
	Material* LoadMaterialFromLibrary(std::string path);
};

