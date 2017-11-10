#pragma once
#include "Module.h"

class Prefab;

class ModulePrefabImporter :
	public Module
{
public:
	ModulePrefabImporter(Application* app, bool start_enabled = true, bool is_game = false);
	~ModulePrefabImporter();

	//Retuns the library path if created or an empty string
	std::string ImportPrefab(std::string path);
	Prefab* LoadPrefabFromLibrary(std::string path);
};

