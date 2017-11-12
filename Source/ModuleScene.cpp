#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleEditor.h"
#include "ModuleRenderer3D.h"
#include "ComponentMeshRenderer.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "GameObject.h"
#include "Component.h"
#include "SceneWindow.h"
#include "PerformanceWindow.h"
#include "Texture.h"
#include "Primitive.h"
#include "ModuleCamera3D.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "Prefab.h"
#include "ModuleResources.h"
#include "Mesh.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled, bool is_game) : Module(app, start_enabled, is_game)
{
	name = "Scene";
	saving_index = 0;
	scene_name = "Untitled Scene";
}

ModuleScene::~ModuleScene()
{}

// Load assets
bool ModuleScene::Start()
{
	CONSOLE_DEBUG("Loading Intro assets");
	bool ret = true;

	math::float3 initial_pos(0.f, 5.f, -10.f);
	App->camera->SetPosition(initial_pos);
	math::float3 initial_look_at(0, 0, 0);
	App->camera->LookAt(initial_look_at);

	octree.Create(float3::zero, float3::zero);
	octree.update_tree = true;
	draw_octree = false;

	mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	mCurrentGizmoMode = ImGuizmo::LOCAL;
	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	CONSOLE_DEBUG("Unloading Scene");
	
	//Destroy the root gameobjects. Each gameobject will take care of their childs
	for (std::list<GameObject*>::iterator it = root_gameobjects.begin(); it != root_gameobjects.end(); ++it) {
		RELEASE(*it);
	}

	return true;
}

GameObject * ModuleScene::CreateGameObject(GameObject * parent)
{
	GameObject* ret = new GameObject(parent);
	RenameDuplicatedGameObject(ret);
	AddGameObjectToScene(ret);
	App->resources->AddGameObject(ret);
	return ret;
}

GameObject * ModuleScene::DuplicateGameObject(GameObject * gameObject)
{
	GameObject* ret = nullptr;

	if (gameObject != nullptr) {
		Data data;
		gameObject->Save(data, true);
		for (int i = 0; i < saving_index; i++) {
			GameObject* go = new GameObject();
			data.EnterSection("GameObject_" + std::to_string(i));
			go->Load(data, true);
			data.LeaveSection();
			scene_gameobjects.push_back(go);
			if (go->IsRoot()) root_gameobjects.push_back(go);
			if (i == 0) { //return the first object (parent)
				ret = go;
			}
			App->resources->AddGameObject(go);
			ComponentTransform* transform = (ComponentTransform*)go->GetComponent(Component::Transform);
			if (transform) transform->UpdateGlobalMatrix();
			ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)go->GetComponent(Component::MeshRenderer);
			if (mesh_renderer) mesh_renderer->LoadToMemory();
		}
		data.ClearData();
		saving_index = 0;
	}

	return ret;
}

update_status ModuleScene::PreUpdate(float dt)
{
	for (std::list<GameObject*>::iterator it = gameobjects_to_destroy.begin(); it != gameobjects_to_destroy.end();) {
		if (*it != nullptr)
		{
			if(!(*it)->GetIsUsedInPrefab()) {
				(*it)->OnDestroy();
				if ((*it)->IsRoot()) {
					root_gameobjects.remove(*it);
				}
				CONSOLE_DEBUG("GameObject Destroyed: %s", (*it)->GetName().c_str());
				RELEASE(*it);
			}
			else
			{
				RemoveWithoutDelete(*it);
			}
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
		ComponentCamera* camera = (ComponentCamera*)(*it)->GetComponent(Component::Camera);
		bool active_parents = RecursiveCheckActiveParents((*it));
		if (active_parents && (*it)->IsActive())
		{
			if (mesh_renderer != nullptr && mesh_renderer->IsActive() && mesh_renderer->GetMesh() != nullptr)
			{
				App->renderer3D->AddMeshToDraw(mesh_renderer);
				if ((*it)->IsSelected())
				{
					DebugAABB aabb(mesh_renderer->GetMesh()->box);
					aabb.Render();
				}
			}
			if (camera != nullptr && camera->IsActive() && (*it)->IsSelected())
			{
				DebugFrustum frustum(camera->GetFrustum());
				frustum.Render();
				App->renderer3D->rendering_cameras.push_back(camera);
			}
		}
	}

	if (octree.update_tree)
	{
		//If octree needs to be updated, set the size to 0 and start growing it
		octree.min_point = float3::zero;
		octree.max_point = float3::zero;
		for (std::list<Mesh*>::iterator it = static_meshes.begin(); it != static_meshes.end(); it++)
		{
			octree.CalculateNewSize((*it)->box.minPoint, (*it)->box.maxPoint);
		}
		//After calculate the size of the new octree, crete it deleteing the previous
		octree.Update();
		//Insert all the contents to the new octree
		for (std::list<Mesh*>::iterator it = static_meshes.begin(); it != static_meshes.end(); it++)
		{
			octree.Insert(&(*it)->box);
		}
		octree.update_tree = false;
	}
	if(draw_octree) octree.DebugDraw();

	App->editor->performance_window->AddModuleData(this->name, ms_timer.ReadMs());
	return UPDATE_CONTINUE;
}

void ModuleScene::AddGameObjectToScene(GameObject* gameobject)
{
	if (gameobject != nullptr)
	{
		scene_gameobjects.push_back(gameobject);

		if (gameobject->GetParent() == nullptr)
			root_gameobjects.push_back(gameobject);

		CONSOLE_DEBUG("GameObject Created: %s", gameobject->GetName().c_str());
	}
	
}

void ModuleScene::AddGameObjectToDestroy(GameObject * gameobject)
{
	gameobjects_to_destroy.push_back(gameobject);
}

void ModuleScene::DestroyAllGameObjects()
{
	for (std::list<GameObject*>::iterator it = root_gameobjects.begin(); it != root_gameobjects.end(); it++)
	{
		if(*it != nullptr) AddGameObjectToDestroy(*it);
	}
}

void ModuleScene::RemoveWithoutDelete(GameObject * gameobject)
{
	if (gameobject->IsRoot())
	{
		root_gameobjects.remove(gameobject);
	}
	scene_gameobjects.remove(gameobject);
	if (std::find(selected_gameobjects.begin(), selected_gameobjects.end(), gameobject) != selected_gameobjects.end()) {
		selected_gameobjects.remove(gameobject);
	}

	ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)gameobject->GetComponent(Component::MeshRenderer);
	if (mesh_renderer)
	{
		Mesh* mesh = mesh_renderer->GetMesh();
		if (mesh)
		{
			if (gameobject->IsStatic())
			{
				if (std::find(static_meshes.begin(), static_meshes.end(), mesh) != static_meshes.end())
				{
					static_meshes.remove(mesh);
				}
			}
			else
			{
				if (std::find(dynamic_meshes.begin(), dynamic_meshes.end(), mesh) != dynamic_meshes.end())
				{
					dynamic_meshes.remove(mesh);
				}
			}
		}
		mesh_renderer->UnloadFromMemory();
	}

	ComponentCamera* camera = (ComponentCamera*)gameobject->GetComponent(Component::Camera);
	if (camera)
	{
		if (std::find(scene_cameras.begin(), scene_cameras.end(), camera) != scene_cameras.end())
		{
			scene_cameras.remove(camera);
		}
	}

	//std::string object_name = gameobject->GetName();
	//scene_gameobjects_name_counter[object_name] -= 1;
	//if (scene_gameobjects_name_counter[object_name] == 0) scene_gameobjects_name_counter.erase(object_name);

	for (std::list<GameObject*>::iterator it = gameobject->childs.begin(); it != gameobject->childs.end(); it++)
	{
		RemoveWithoutDelete(*it);
	}
}

void ModuleScene::ApplyTextureToSelectedGameObjects(Texture * texture)
{
	for (std::list<GameObject*>::iterator it = selected_gameobjects.begin(); it != selected_gameobjects.end(); it++)
	{
		if (*it != nullptr) ApplyTextureToGameObject(*it, texture);
	}
}

void ModuleScene::ApplyTextureToGameObject(GameObject * gameobject, Texture* texture)
{
	if (gameobject->IsActive())
	{
		ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)gameobject->GetComponent(Component::MeshRenderer);

		if (mesh_renderer != nullptr)
		{
			//mesh_renderer->SetTexture(texture);
			CONSOLE_DEBUG("Texture %s attached to %s", texture->GetName().c_str(), gameobject->GetName().c_str());
		}
	}
}

int ModuleScene::GetNumCameras() const
{
	return scene_cameras.size();
}

void ModuleScene::InsertGoInOctree(AABB& box)
{
	octree.Insert(&box);
}

void ModuleScene::EraseGoInOctree(AABB& box)
{
	octree.Erase(&box);
}

void ModuleScene::NewScene()
{
	gameobjects_to_destroy = root_gameobjects;
	scene_gameobjects.clear();
	scene_gameobjects_name_counter.clear();
	root_gameobjects.clear();
	selected_gameobjects.clear();
	scene_cameras.clear();
	static_meshes.clear();
	dynamic_meshes.clear();
	App->window->SetTitle(DEFAULT_SCENE_TITLE);
	octree.Clear();
	octree.Create(float3::zero, float3::zero);
	octree.update_tree = true;
}

void ModuleScene::LoadScene(std::string path)
{
	Data data;
	if (data.LoadBinary(path)) {
		NewScene();
		scene_name = data.GetString("Scene Name");
		App->window->SetTitle((SCENE_TITLE_PREFIX + scene_name).c_str());
		int gameObjectsCount = data.GetInt("GameObjects_Count");
		for (int i = 0; i < gameObjectsCount; i++) {
			data.EnterSection("GameObject_" + std::to_string(i));
			GameObject* game_object = new GameObject();
			game_object->Load(data);
			data.LeaveSection();
			scene_gameobjects.push_back(game_object);
			if (game_object->IsRoot()) root_gameobjects.push_back(game_object);
			App->resources->AddGameObject(game_object);
			ComponentTransform* transform = (ComponentTransform*)game_object->GetComponent(Component::Transform);
			if (transform) transform->UpdateGlobalMatrix();
			ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)game_object->GetComponent(Component::MeshRenderer);
			if (mesh_renderer) mesh_renderer->LoadToMemory();
		}
	}
	else
	{
		CONSOLE_ERROR("Cannot load %s scene", path.c_str());
	}
}

void ModuleScene::SaveScene(std::string path) const
{
	Data data;
	data.AddString("Scene Name", scene_name);
	data.AddInt("GameObjects_Count", scene_gameobjects.size());
	for (std::list<GameObject*>::const_iterator it = root_gameobjects.begin(); it != root_gameobjects.end(); it++) {
		(*it)->Save(data);
	}
	data.SaveAsBinary(path);
}

void ModuleScene::LoadPrefab(Prefab* prefab)
{
	GameObject* prefab_root = prefab->GetRootGameObject();
	if (std::find(root_gameobjects.begin(), root_gameobjects.end(), prefab_root) == root_gameobjects.end())
	{
		std::vector<GameObject*> createdObjects;
		bool justIncrease = false;
		if (scene_gameobjects.empty()) {
		justIncrease = true;
		}

		std::vector<GameObject*> prefab_gameobjects = prefab->GetGameObjects();
		for (std::vector<GameObject*>::iterator it = prefab_gameobjects.begin(); it != prefab_gameobjects.end(); it++)
		{

		AddGameObjectToScene(*it);
		createdObjects.push_back(*it);
		ComponentTransform* transform = (ComponentTransform*)(*it)->GetComponent(Component::Transform);
		if (transform) transform->UpdateGlobalMatrix();
		ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)(*it)->GetComponent(Component::MeshRenderer);
		if (mesh_renderer) mesh_renderer->LoadToMemory();
		}

		for (int i = 0; i < createdObjects.size(); i++) {
		RenameDuplicatedGameObject(createdObjects[i], justIncrease);
		}
	}
	else
	{
		DuplicateGameObject(prefab_root);
	}
}

bool ModuleScene::RecursiveCheckActiveParents(GameObject* gameobject)
{
	bool ret = true;
	if (gameobject->GetParent() != nullptr)
	{
		if (gameobject->GetParent()->IsActive())
		{
			ret = RecursiveCheckActiveParents(gameobject->GetParent());
		}
		else {
			ret = false;
		}
	}
	return ret;
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
		//Gizmo
		if (App->camera->can_update == false)
		{
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
			{
				mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
			}
			if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				mCurrentGizmoOperation = ImGuizmo::ROTATE;
			}
			if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
			{
				mCurrentGizmoOperation = ImGuizmo::SCALE;
			}
		}
		//Focus on first selected object
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			if (!selected_gameobjects.empty())
			{
				ComponentTransform* transform = (ComponentTransform*)selected_gameobjects.front()->GetComponent(Component::Transform);
				App->camera->can_update = true;
				App->camera->LookAt(transform->GetGlobalPosition());
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
					App->camera->LookAt(transform->GetGlobalPosition());
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
	if (gameObject == nullptr) return;

	int gameObjectCount = 1;
	//Rename if name exist
	bool inParenthesis = false;
	std::string object_name = gameObject->GetName();
	std::string str;
	for (int i = 0; i < object_name.size(); i++) {
		if (object_name[i] == ')') {
			inParenthesis = false;
			if (object_name[i + 1] == '\0') {
				break;
			}
			else {
				str.clear();
			}
		}
		if (inParenthesis) {
			str.push_back(object_name[i]);
		}
		if (object_name[i] == '(') {
			inParenthesis = true;
		}
	}
	if (atoi(str.c_str()) != 0) {
		object_name.erase(object_name.end() - (str.length() + 2), object_name.end());
		gameObjectCount = stoi(str);
	}

	std::map<std::string, int>::iterator it = scene_gameobjects_name_counter.find(object_name);
	if (it != scene_gameobjects_name_counter.end()) {
		if (scene_gameobjects_name_counter[object_name] < gameObjectCount && !justIncrease) {
			scene_gameobjects_name_counter[object_name] = gameObjectCount;
		}
		else {
			scene_gameobjects_name_counter[object_name] += 1;
		}
		gameObject->SetName(object_name + "(" + std::to_string(it->second) + ")");
	}
	else {
		if (gameObjectCount > 1) {
			scene_gameobjects_name_counter[object_name] = gameObjectCount;
			gameObject->SetName(object_name + "(" + std::to_string(gameObjectCount) + ")");
		}
		else {
			scene_gameobjects_name_counter[object_name] = 1;
		}
	}
}

GameObject * ModuleScene::FindGameObject(uint id) const
{
	return App->resources->GetGameObject(id);
}
