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
	data.AddInt("Type", GetType());
	data.AddBool("Active", IsActive());
	data.AddUInt("UUID", GetUID());
	data.CreateSection("Mesh");
	if(mesh) mesh->Save(data);
	data.CloseSection();
	data.CreateSection("Texture");
	if(texture)texture->Save(data);
	data.CloseSection();
}

void ComponentMeshRenderer::Load(Data & data)
{
	SetType((Component::ComponentType)data.GetInt("Type"));
	SetActive(data.GetBool("Active"));
	SetUID(data.GetUInt("UUID"));
	data.EnterSection("Mesh");

}


