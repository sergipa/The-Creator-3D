#pragma once
#include "Resource.h"

class Prefab :
	public Resource
{
public:
	Prefab();
	~Prefab();

	void Save(Data& data) const;
	bool Load(Data& data);
	void CreateMeta() const;
	void LoadToMemory();
	void UnloadFromMemory();
};

