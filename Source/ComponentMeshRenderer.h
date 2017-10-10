#pragma once
#include "Component.h"
#include "Mesh.h"

class ComponentMeshRenderer :
	public Component
{
public:
	ComponentMeshRenderer(GameObject* attached_gameobject);
	~ComponentMeshRenderer();

	Mesh* GetMesh();

	void Save(Data& data) const;
	void Load(Data& data);

private:
	Mesh* mesh;
};

