#pragma once
#include "Component.h"
#include <list>
#include <map>

class Prefab;
class GameObject;

class ComponentFactory :
	public Component
{
public:

	ComponentFactory(GameObject* attached_gameobject);
	~ComponentFactory();

	void SetFactoryObject(Prefab* gameobject);
	void SetObjectCount(int count);
	void SetSpawnPos(float3 position);
	void SetSpawnRotation(float3 rotation);
	void SetSpawnScale(float3 scale);
	void SetLifeTime(float life_time);
	void Spawn();
	int GetCurrentCount() const;
	float GetLifeTime() const;
	Prefab* GetFactoryObject() const;
	int GetObjectCount() const;
	float3 GetSpawnPosition() const;
	float3 GetSpawnRotation() const;
	float3 GetSpawnScale() const;
	void StartFactory();

	void Save(Data& data) const;
	void Load(Data& data);

private:
	void CheckLifeTimes();

private:
	Prefab* object_to_spawn;
	int object_count;
	float3 spawn_position;
	float3 spawn_rotation;
	float3 spawn_scale;
	float life_time;
	float3 original_position;

	std::list<Prefab*> spawn_objects_list;
	std::map<Prefab*, float> spawned_objects;
};

