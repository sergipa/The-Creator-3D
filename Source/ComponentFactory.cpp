#include "ComponentFactory.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ModuleTime.h"
#include "Application.h"
#include "Prefab.h"
#include "ModuleResources.h"

ComponentFactory::ComponentFactory(GameObject* attached_gameobject)
{
	SetActive(true);
	SetName("Factory");
	SetType(ComponentType::CompFactory);
	SetGameObject(attached_gameobject);

	if (attached_gameobject != nullptr)
	{
		ComponentTransform* transform = (ComponentTransform*)attached_gameobject->GetComponent(Component::CompTransform);
		if (transform)
		{
			original_position = transform->GetGlobalPosition();
		}
	}

	object_to_spawn = nullptr;
	object_count = 0;
	life_time = 0;
}

ComponentFactory::~ComponentFactory()
{
}

void ComponentFactory::SetFactoryObject(Prefab * gameobject)
{
	if (gameobject)
	{
		object_to_spawn = gameobject;
	}
	else
	{
		CONSOLE_ERROR("Prefab assigned to factory is null");
	}
}

void ComponentFactory::SetObjectCount(int count)
{
	object_count = count;
}

void ComponentFactory::SetSpawnPos(float3 position)
{
	spawn_position = position;
}

void ComponentFactory::SetSpawnRotation(float3 rotation)
{
	spawn_rotation = rotation;
}

void ComponentFactory::SetSpawnScale(float3 scale)
{
	spawn_scale = scale;
}

void ComponentFactory::SetLifeTime(float life_time)
{
	this->life_time = life_time;
}

void ComponentFactory::Spawn()
{
	GameObject* go = spawn_objects_list.front()->GetRootGameObject();
	if (go != nullptr)
	{
		ComponentTransform* transform = (ComponentTransform*)go->GetComponent(Component::CompTransform);
		if (transform)
		{
			transform->SetPosition(spawn_position);
			transform->SetRotation(spawn_rotation);
			transform->SetScale(spawn_scale);
			go->SetActive(true);
			spawned_objects[spawn_objects_list.front()] = life_time;
		}
	}
	else
	{
		CONSOLE_ERROR("Spawned GameObject from Factory component in %s", GetGameObject()->GetName());
	}

	CheckLifeTimes();
}

int ComponentFactory::GetCurrentCount() const
{
	return spawn_objects_list.size();
}

float ComponentFactory::GetLifeTime() const
{
	return life_time;
}

Prefab * ComponentFactory::GetFactoryObject() const
{
	return object_to_spawn;
}

int ComponentFactory::GetObjectCount() const
{
	return object_count;
}

float3 ComponentFactory::GetSpawnPosition() const
{
	return spawn_position;
}

float3 ComponentFactory::GetSpawnRotation() const
{
	return spawn_rotation;
}

float3 ComponentFactory::GetSpawnScale() const
{
	return spawn_scale;
}

void ComponentFactory::StartFactory()
{
	if (object_to_spawn)
	{
		for (int i = 0; i < object_count; i++)
		{
			Prefab* new_tmp = new Prefab();
			GameObject* new_root = new GameObject();
			Data data;
			object_to_spawn->GetRootGameObject()->Save(data, true);
			new_root->Load(data);
		}
	}
	else
	{
		CONSOLE_ERROR("%s Factory component is trying to spawn a null prefab", GetGameObject()->GetName().c_str());
	}
}

void ComponentFactory::CheckLifeTimes()
{
	for (std::map<Prefab*, float>::iterator it = spawned_objects.begin(); it != spawned_objects.end();)
	{
		if (it->second <= 0)
		{
			ComponentTransform* transform = (ComponentTransform*)it->first->GetRootGameObject()->GetComponent(Component::CompTransform);
			if (transform)
			{
				it->first->GetRootGameObject()->SetActive(false);
				transform->SetPosition(original_position);
				it = spawned_objects.erase(it);
			}
		}
		else
		{
			it++;
		}
	}
}

void ComponentFactory::Save(Data & data) const
{
	data.AddInt("Object_count", object_count);
	data.AddVector3("Spawn_position", spawn_position);
	data.AddVector3("Spawn_rotation", spawn_rotation);
	data.AddVector3("Spawn_scale", spawn_scale);
	data.AddFloat("Life_time", life_time);
	int object_uid = 0;
	if (object_to_spawn)
	{
		object_uid = object_to_spawn->GetUID();
	}
	data.AddInt("Prefab_UID", object_uid);
}

void ComponentFactory::Load(Data & data)
{
	object_count = data.GetInt("Object_count");
	spawn_position = data.GetVector3("Spawn_position");
	spawn_rotation = data.GetVector3("Spawn_rotation");
	spawn_scale = data.GetVector3("Spawn_scale");
	life_time = data.GetFloat("Life_time");
	object_to_spawn = App->resources->GetPrefab(data.GetInt("Prefab_UID"));
	if (!object_to_spawn)
	{
		CONSOLE_WARNING("Prefab loaded for component Factory in %s doesn't exist", GetGameObject()->GetName().c_str());
	}
}
