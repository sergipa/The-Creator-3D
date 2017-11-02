#pragma once
#include "Module.h"
#include "MathGeoLib\MathBuildConfig.h"
#include "MathGeoLib\MathGeoLib.h"
#include "Primitive.h"
#include <map>
#include <string>
#include "Octree.h"

class GameObject;
class Texture;
class ComponentCamera;

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
	//GameObject* DuplicateGameObject(GameObject* gameObject);
	void RenameDuplicatedGameObject(GameObject * gameObject, bool justIncrease = false);

	void AddGameObjectToScene(GameObject* gameobject);
	void AddGameObjectToDestroy(GameObject* gameobject);
	void DestroyAllGameObjects();

	void ApplyTextureToSelectedGameObjects(Texture* texture);
	void ApplyTextureToGameObject(GameObject* gameobject, Texture* texture);

	int GetNumCameras() const;

private:
	bool RecursiveCheckActiveParents(GameObject* gameobject);
	void HandleInput();

public:
	std::list<GameObject*> selected_gameobjects;
	std::list<GameObject*> root_gameobjects;
	std::list<GameObject*> scene_gameobjects;
	std::list<ComponentCamera*> scene_cameras;
	Octree octree;
	bool draw_octree;
private:
	
	std::list<GameObject*> gameobjects_to_destroy;
	std::map<std::string, int> scene_gameobjects_name_counter;
};
