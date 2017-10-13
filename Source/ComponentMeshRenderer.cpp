#include "ComponentMeshRenderer.h"


ComponentMeshRenderer::ComponentMeshRenderer(GameObject* attached_gameobject)
{
	SetActive(true);
	SetName("Mesh_Renderer");
	SetType(ComponentType::MeshRenderer);
	SetGameObject(attached_gameobject);
	mesh = nullptr;
	texture = nullptr;
}

ComponentMeshRenderer::~ComponentMeshRenderer()
{
	delete mesh;
	delete texture;
}

Mesh* ComponentMeshRenderer::GetMesh() const
{
	return mesh;
}

void ComponentMeshRenderer::LoadMesh(Mesh * mesh)
{
	this->mesh = mesh;
}

Texture * ComponentMeshRenderer::GetTexture() const
{
	return texture;
}

void ComponentMeshRenderer::LoadTexture(Texture * texture)
{
	this->texture = texture;
}

void ComponentMeshRenderer::Save(Data & data) const
{
}

void ComponentMeshRenderer::Load(Data & data)
{
}


