#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleEditor.h"
#include "ModuleRenderer3D.h"
#include "ComponentMeshRenderer.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "Component.h"
#include "SceneWindow.h"
#include "PerformanceWindow.h"
#include "Texture.h"

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

update_status ModuleScene::PreUpdate(float dt)
{
	for (std::list<GameObject*>::iterator it = gameobjects_to_destroy.begin(); it != gameobjects_to_destroy.end();) {
		if (*it != nullptr) {
			(*it)->OnDestroy();
			if ((*it)->IsRoot()) {
				root_gameobjects.remove(*it);
			}
			selected_gameobjects.remove(*it);
			RELEASE(*it);
			it = gameobjects_to_destroy.erase(it);
		}
	}

	return UPDATE_CONTINUE;
}

// Update
update_status ModuleScene::Update(float dt)
{
	ms_timer.Start();

	HandleInput();
	
	for (std::list<GameObject*>::iterator it = scene_gameobjects.begin(); it != scene_gameobjects.end(); it++)
	{
		ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)(*it)->GetComponent(Component::MeshRenderer);
		bool active_parents = RecursiveCheckActiveParents((*it));
		if (mesh_renderer != nullptr && mesh_renderer->IsActive() && active_parents && (*it)->IsActive() && mesh_renderer->GetMesh() != nullptr)
		{
			App->renderer3D->AddMeshToDraw(mesh_renderer);
		}
	}

	App->editor->performance_window->AddModuleData(this->name, ms_timer.ReadMs());
	return UPDATE_CONTINUE;
}

void ModuleScene::AddGameObjectToScene(GameObject* gameobject)
{
	scene_gameobjects.push_back(gameobject);

	if(gameobject->GetParent()== nullptr)
		root_gameobjects.push_back(gameobject);
}

void ModuleScene::AddGameObjectToDestroy(GameObject * gameobject)
{
	gameobjects_to_destroy.push_back(gameobject);
}

void ModuleScene::ApplyTextureToSelectedGameObjects(Texture * texture)
{
	for (std::list<GameObject*>::iterator it = selected_gameobjects.begin(); it != selected_gameobjects.end(); it++)
	{
		ApplyTextureToGameObject((*it), texture);
	}
}

void ModuleScene::ApplyTextureToGameObject(GameObject * gameobject, Texture* texture)
{
	if (gameobject->IsActive())
	{
		ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)gameobject->GetComponent(Component::MeshRenderer);

		if (mesh_renderer != nullptr)
		{
			mesh_renderer->LoadTexture(texture);
		}
	}
}

bool ModuleScene::RecursiveCheckActiveParents(GameObject* gameobject)
{
	if (gameobject->GetParent() != nullptr)
	{
		if (gameobject->GetParent()->IsActive())
		{
			RecursiveCheckActiveParents(gameobject->GetParent());
		}
		else {
			return false;
		}
	}
	return true;
}

void ModuleScene::HandleInput()
{
	if (App->editor->scene_window->IsWindowFocused() && App->editor->scene_window->IsMouseHoveringWindow())
	{
		//Rotate camera or zomm in/out
		if (App->input->GetMouseButton(3) == KEY_REPEAT || App->input->GetMouseZ() > 0 || App->input->GetMouseZ() < 0)
		{
			App->camera->can_update = true;
		}
		if (App->input->GetMouseButton(3) == KEY_UP)
		{
			App->camera->can_update = false;
		}
		if (App->input->GetMouseButton(3) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_IDLE && 
			App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_IDLE && App->input->GetMouseZ() == 0)
		{
			App->camera->can_update = false;
		}

		//Focus on first selected object
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			if (!selected_gameobjects.empty())
			{
				ComponentTransform* transform = (ComponentTransform*)selected_gameobjects.front()->GetComponent(Component::Transform);
				App->camera->can_update = true;
				App->camera->Look(App->camera->Position, transform->GetGlobalPosition());
				App->camera->can_update = false;
			}
		} 
		//Use orbital camera
		else if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{			
			if (!App->camera->IsOrbital())
			{
				if (!selected_gameobjects.empty())
				{
					ComponentTransform* transform = (ComponentTransform*)selected_gameobjects.front()->GetComponent(Component::Transform);
					App->camera->can_update = true;
					App->camera->Look(App->camera->Position, transform->GetGlobalPosition(), true);
					App->camera->SetOrbital(true);
				}
			}
		}
		//Disable orbital camera
		if (App->camera->IsOrbital() && (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_UP || App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP))
		{
			App->camera->can_update = false;
			App->camera->SetOrbital(false);
		}
	}
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