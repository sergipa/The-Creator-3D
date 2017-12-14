#include "ModulePrefabImporter.h"
#include "Prefab.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "Data.h"

ModulePrefabImporter::ModulePrefabImporter(Application* app, bool start_enabled, bool is_game) : Module(app, start_enabled, is_game)
{
}

ModulePrefabImporter::~ModulePrefabImporter()
{
}

std::string ModulePrefabImporter::ImportPrefab(std::string path)
{
	std::string file_name = App->file_system->GetFileName(path);
	std::string library_path = LIBRARY_PREFABS_FOLDER + file_name;
	App->file_system->Copy(path, library_path);
	return library_path;
}

Prefab * ModulePrefabImporter::LoadPrefabFromLibrary(std::string path)
{
	Prefab* prefab = new Prefab();

	Data data;
	if (data.LoadBinary(path)) {
		prefab->Load(data);
	}
	return prefab;
}
