#include "ComponentTransform.h"
#include "GameObject.h"

ComponentTransform::ComponentTransform(GameObject* attached_gameobject)
{
	SetActive(true);
	SetName("Transform");
	SetType(ComponentType::Transform);
	SetGameObject(attached_gameobject);

	position = float3(0,0,0);
	rotation = float3(0,0,0);
	scale = float3(1,1,1);
	transform_matrix.SetIdentity();
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::SetPosition(float3 position)
{
	this->position = position;
	void UpdateGlobalMatrix();
}

float3 ComponentTransform::GetGlobalPosition() const
{
	return position;
}

float3 ComponentTransform::GetLocalPosition() const
{
	if (GetGameObject()->GetParent() != nullptr)
	{
		ComponentTransform* parent_transform = (ComponentTransform*)GetGameObject()->GetParent()->GetComponent(ComponentType::Transform);
		return GetGlobalPosition() - parent_transform->GetGlobalPosition();
	}
	return GetGlobalPosition(); //If it's the parent. local position = global position
}

void ComponentTransform::SetRotation(float3 rotation)
{
	this->rotation = rotation;
	void UpdateGlobalMatrix();
}

float3 ComponentTransform::GetGlobalRotation() const
{
	return rotation;
}

float3 ComponentTransform::GetLocalRotation() const
{
	if (GetGameObject()->GetParent() != nullptr)
	{
		ComponentTransform* parent_transform = (ComponentTransform*)GetGameObject()->GetParent()->GetComponent(ComponentType::Transform);
		return GetGlobalRotation() - parent_transform->GetGlobalRotation();
	}
	return GetGlobalRotation(); //If it's the parent. local rotation = global rotation
}

void ComponentTransform::SetScale(float3 scale)
{
	this->scale = scale;
	void UpdateGlobalMatrix();
}

float3 ComponentTransform::GetGlobalScale() const
{
	return scale;
}

float3 ComponentTransform::GetLocalScale() const
{
	if (GetGameObject()->GetParent() != nullptr)
	{
		ComponentTransform* parent_transform = (ComponentTransform*)GetGameObject()->GetParent()->GetComponent(ComponentType::Transform);
		return (GetGlobalScale() - parent_transform->GetGlobalScale()) + float3(1,1,1); //scale is 1,1,1 if not modified
	}
	return GetGlobalScale(); //If it's the parent. local scale = global scale
}

void ComponentTransform::UpdateGlobalMatrix()
{
	transform_matrix.SetIdentity();
	transform_matrix.Scale(scale);
	transform_matrix.RotateX(rotation.x);
	transform_matrix.RotateY(rotation.y);
	transform_matrix.RotateZ(rotation.z);
	transform_matrix.Translate(position);
}

void ComponentTransform::Save(Data & data) const
{
}

void ComponentTransform::Load(Data & data)
{
}
