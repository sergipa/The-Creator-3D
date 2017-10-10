#include "ComponentMeshRenderer.h"


ComponentMeshRenderer::ComponentMeshRenderer(GameObject* attached_gameobject)
{
	SetActive(true);
	SetName("Mesh_Renderer");
	SetType(ComponentType::MeshRenderer);
	SetGameObject(attached_gameobject);
	mesh = nullptr;
}

ComponentMeshRenderer::~ComponentMeshRenderer()
{
}

Mesh* ComponentMeshRenderer::GetMesh()
{
	return mesh;
}

void ComponentMeshRenderer::Save(Data & data) const
{
}

void ComponentMeshRenderer::Load(Data & data)
{
}


