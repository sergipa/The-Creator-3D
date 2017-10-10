#pragma once
#include "Module.h"
#include "MathGeoLib\MathBuildConfig.h"
#include "MathGeoLib\MathGeoLib.h"
#include "Primitive.h"
#include <map>
#include <string>

class GameObject;


class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	GameObject* CreateGameObject(GameObject* parent = nullptr);
	void RenameDuplicatedGameObject(GameObject * gameObject, bool justIncrease = false);

	void AddGameObjectToScene(GameObject* gameobject);
	void AddGameObjectToDestroy(GameObject* gameobject);

public:
	std::list<GameObject*> selected_gameobjects;
	std::list<GameObject*> root_gameobjects;

private:
	std::list<GameObject*> scene_gameobjects;
	std::list<GameObject*> gameobjects_to_destroy;
	std::map<std::string, int> scene_gameobjects_name_counter;
};
