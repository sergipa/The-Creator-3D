#include "ComponentMeshRenderer.h"
#include "GameObject.h"

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

}

Mesh* ComponentMeshRenderer::GetMesh() const
{
	return mesh;
}

void ComponentMeshRenderer::LoadMesh(Mesh * mesh)
{
	this->mesh = mesh;
	UpdateBoundingBox();
}

Texture * ComponentMeshRenderer::GetTexture() const
{
	return texture;
}

void ComponentMeshRenderer::LoadTexture(Texture * texture)
{
	this->texture = texture;
}

void ComponentMeshRenderer::UpdateBoundingBox()
{
	if (GetMesh() != nullptr)
	{
		GetMesh()->box.SetNegativeInfinity();
		GetMesh()->box.Enclose((float3*)GetMesh()->vertices, GetMesh()->num_vertices);
		math::OBB obb = GetMesh()->box.Transform(GetGameObject()->GetGlobalTransfomMatrix());
		GetMesh()->box = obb.MinimalEnclosingAABB();
	}
}

void ComponentMeshRenderer::Save(Data & data) const
{
}

void ComponentMeshRenderer::Load(Data & data)
{
}


