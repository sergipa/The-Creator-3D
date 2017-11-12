#pragma once
#include "Component.h"

class Mesh;
class Material;

class ComponentMeshRenderer :
	public Component
{
public:
	ComponentMeshRenderer(GameObject* attached_gameobject);
	virtual ~ComponentMeshRenderer();

	Mesh* GetMesh() const;
	void SetMesh(Mesh* mesh);

	Material* GetMaterial() const;
	void SetMaterial(Material* texture);

	void UpdateBoundingBox();

	void LoadToMemory();
	void UnloadFromMemory();

	void Save(Data& data) const;
	void Load(Data& data);

private:
	Mesh* mesh;
	Material* material;
};

