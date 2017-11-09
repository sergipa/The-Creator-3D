#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Texture.h"

class ComponentMeshRenderer :
	public Component
{
public:
	ComponentMeshRenderer(GameObject* attached_gameobject);
	virtual ~ComponentMeshRenderer();

	Mesh* GetMesh() const;
	void LoadMesh(Mesh* mesh);

	Texture* GetTexture() const;
	void LoadTexture(Texture* texture);

	void UpdateBoundingBox();

	void LoadToMemory();

	void Save(Data& data) const;
	void Load(Data& data);

private:
	Mesh* mesh;
	Texture* texture;
};

