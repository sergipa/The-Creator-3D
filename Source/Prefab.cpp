#include "Prefab.h"
#include "Data.h"
#include <ctime>
#include <vector>
#include "GameObject.h"

Prefab::Prefab()
{
	SetType(Resource::PrefabResource);
	SetUsedCount(0);
}

Prefab::~Prefab()
{
}

void Prefab::AddGameObject(GameObject * game_object)
{
	prefab_gameobjects.push_back(game_object);
}

std::vector<GameObject*> Prefab::GetGameObjects() const
{
	return prefab_gameobjects;
}

void Prefab::Save(Data & data) const
{
}

bool Prefab::Load(Data & data)
{
	
	return true;
}

void Prefab::CreateMeta() const
{
	time_t now = time(0);
	char* dt = ctime(&now);

	Data data;
	data.AddInt("Type", GetType());
	data.AddUInt("UUID", GetUID());
	data.AddString("Time_Created", dt);
	data.AddString("Library_path", GetLibraryPath());
	data.SaveAsMeta(GetAssetsPath());
}

void Prefab::LoadToMemory()
{
}

void Prefab::UnloadFromMemory()
{
}
