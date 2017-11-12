#pragma once
#include "Module.h"
#include "MathGeoLib\MathBuildConfig.h"
#include "MathGeoLib\MathGeoLib.h"
#include "Primitive.h"
#include <map>
#include <string>
#include "Octree.h"
#include "imgui\ImGuizmo\ImGuizmo.h"

class GameObject;
class Texture;
class ComponentCamera;
class Mesh;
class Prefab;

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true, bool is_game = false);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	update_status PreUpdate(float dt);
	bool CleanUp();

	GameObject* CreateGameObject(GameObject* parent = nullptr);
	GameObject* DuplicateGameObject(GameObject* gameObject);
	void RenameDuplicatedGameObject(GameObject * gameObject, bool justIncrease = false);
	GameObject* FindGameObject(uint id) const;

	void AddGameObjectToScene(GameObject* gameobject);
	void AddGameObjectToDestroy(GameObject* gameobject);
	void DestroyAllGameObjects();
	void RemoveWithoutDelete(GameObject* gameobject);

	void ApplyTextureToSelectedGameObjects(Texture* texture);
	void ApplyTextureToGameObject(GameObject* gameobject, Texture* texture);

	int GetNumCameras() const;

	void InsertGoInOctree(AABB& box);
	void EraseGoInOctree(AABB& box);

	void NewScene();
	void LoadScene(std::string path);
	void SaveScene(std::string path) const;

	void LoadPrefab(Prefab* prefab);

private:
	bool RecursiveCheckActiveParents(GameObject* gameobject);
	void HandleInput();

public:
	std::list<GameObject*> selected_gameobjects;
	std::list<GameObject*> root_gameobjects;
	std::list<GameObject*> scene_gameobjects;
	std::map<std::string, int> scene_gameobjects_name_counter;
	std::list<ComponentCamera*> scene_cameras;
	std::list<Mesh*> static_meshes;
	std::list<Mesh*> dynamic_meshes;
	Octree octree;
	bool draw_octree;
	int saving_index;
	std::string scene_name;

	ImGuizmo::OPERATION mCurrentGizmoOperation;
	ImGuizmo::MODE mCurrentGizmoMode;
private:
	
	std::list<GameObject*> gameobjects_to_destroy;
};
