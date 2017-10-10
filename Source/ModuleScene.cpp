#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleEditor.h"
#include "ModuleRenderer3D.h"
#include "ComponentMeshRenderer.h"
#include "GameObject.h"
#include "Component.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Scene";
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	CONSOLE_LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	CONSOLE_LOG("Unloading Intro scene");

	return true;
}

GameObject * ModuleScene::CreateGameObject(GameObject * parent)
{
	GameObject* ret = new GameObject(parent);
	if (parent == nullptr) {
		root_gameobjects.push_back(ret);
	}

	RenameDuplicatedGameObject(ret);
	scene_gameobjects.push_back(ret);
	return ret;
}

GameObject * ModuleScene::DuplicateGameObject(GameObject * gameObject)
{
	GameObject* ret = nullptr;

	//if (gameObject != nullptr) {
	//	Data data;
	//	gameObject->Save(data, true);
	//	for (int i = 0; i < savingIndex; i++) {
	//		GameObject* go = new GameObject();
	//		data.EnterSection("GameObject_" + std::to_string(i));
	//		go->Load(data);
	//		data.LeaveSection();
	//		gameobjectsList.push_back(go);
	//		engine->sceneWindow->drawableObjects.push_back(go);
	//		if (i == 0) { //return the first object (parent)
	//			ret = go;
	//		}
	//	}
	//	data.ClearData();
	//	engine->sceneManagerModule->savingIndex = 0;
	//}

	return ret;
}

// Update
update_status ModuleScene::Update(float dt)
{
	ms_timer.Start();
	
	for (std::list<GameObject*>::iterator it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		if ((*it)->IsActive())
		{
			ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)(*it)->GetComponent(Component::MeshRenderer);
			if (mesh_renderer != nullptr)
			{
				App->renderer3D->AddMeshToDraw(mesh_renderer->GetMesh());
			}
		}
	}

	App->editor->SendDataToPerformance(this->name, ms_timer.ReadMs());
	return UPDATE_CONTINUE;
}

void ModuleScene::AddGameObjectToScene(GameObject* gameobject)
{
	scene_gameobjects.push_back(gameobject);
	root_gameobjects.push_back(gameobject);
}

void ModuleScene::AddGameObjectToDestroy(GameObject * gameobject)
{
	gameobjects_to_destroy.push_back(gameobject);
}


void ModuleScene::RenameDuplicatedGameObject(GameObject * gameObject, bool justIncrease)
{
	int gameObjectCount = 1;
	//Rename if name exist
	bool inParenthesis = false;
	std::string str;
	for (int i = 0; i < gameObject->GetName().size(); i++) {
		if (gameObject->GetName()[i] == ')') {
			inParenthesis = false;
			if (gameObject->GetName()[i + 1] == '\0') {
				break;
			}
			else {
				str.clear();
			}
		}
		if (inParenthesis) {
			str.push_back(gameObject->GetName()[i]);
		}
		if (gameObject->GetName()[i] == '(') {
			inParenthesis = true;
		}
	}
	if (atoi(str.c_str()) != 0) {
		gameObject->GetName().erase(gameObject->GetName().end() - (str.length() + 2), gameObject->GetName().end());
		gameObjectCount = stoi(str);
	}

	std::map<std::string, int>::iterator it = scene_gameobjects_name_counter.find(gameObject->GetName());
	if (it != scene_gameobjects_name_counter.end()) {
		if (scene_gameobjects_name_counter[gameObject->GetName()] < gameObjectCount && !justIncrease) {
			scene_gameobjects_name_counter[gameObject->GetName()] = gameObjectCount;
		}
		else {
			scene_gameobjects_name_counter[gameObject->GetName()] += 1;
		}
		gameObject->SetName(gameObject->GetName() + "(" + std::to_string(it->second) + ")");
	}
	else {
		if (gameObjectCount > 1) {
			scene_gameobjects_name_counter[gameObject->GetName()] = gameObjectCount;
			gameObject->SetName(gameObject->GetName() + "(" + std::to_string(gameObjectCount) + ")");
		}
		else {
			scene_gameobjects_name_counter[gameObject->GetName()] = 1;
		}
	}
}