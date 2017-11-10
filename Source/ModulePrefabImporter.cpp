#include "ModulePrefabImporter.h"
#include "Prefab.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "Data.h"
#include "GameObject.h"

ModulePrefabImporter::ModulePrefabImporter(Application* app, bool start_enabled, bool is_game) : Module(app, start_enabled, is_game)
{
}

ModulePrefabImporter::~ModulePrefabImporter()
{
}

std::string ModulePrefabImporter::ImportPrefab(std::string path)
{
	std::string file_name = App->file_system->GetFileName(path);
	std::string library_path = LIBRARY_PREFABS_FOLDER_PATH + file_name;
	App->file_system->Copy_File(path, library_path);
	return library_path;
}

Prefab * ModulePrefabImporter::LoadPrefabFromLibrary(std::string path)
{
	Prefab* prefab = new Prefab();

	Data data;
	if (data.LoadBinary(path)) {
		int gameObjectsCount = data.GetInt("GameObjectsCount");
		for (int i = 0; i < gameObjectsCount; i++) {
			GameObject* go = new GameObject();
			data.EnterSection("GameObject_" + std::to_string(i));
			go->Load(data, true);
			data.LeaveSection();
			prefab->AddGameObject(go);
		}
		prefab->SetLibraryPath(path);
		prefab->SetName(data.GetString("Name"));
	}
	return prefab;
}
