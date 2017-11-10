#pragma once
#include "Resource.h"
#include <vector>

class Data;
class GameObject;

class Prefab :
	public Resource
{
public:
	Prefab();
	~Prefab();

	void AddGameObject(GameObject* game_object);
	std::vector<GameObject*> GetGameObjects() const;

	void Save(Data& data) const;
	bool Load(Data& data);
	void CreateMeta() const;
	void LoadToMemory();
	void UnloadFromMemory();

private:
	std::vector<GameObject*> prefab_gameobjects;
};

