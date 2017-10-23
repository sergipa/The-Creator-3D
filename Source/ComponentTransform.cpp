#include "ComponentTransform.h"
#include "GameObject.h"

ComponentTransform::ComponentTransform(GameObject* attached_gameobject)
{
	SetActive(true);
	SetName("Transform");
	SetType(ComponentType::Transform);
	SetGameObject(attached_gameobject);

	position = float3(0.f, 0.f, 0.f);
	shown_rotation = float3(0.f, 0.f, 0.f);
	rotation = Quat(0.f, 0.f, 0.f, 0.f);
	scale = float3(1.f, 1.f, 1.f);
	transform_matrix.SetIdentity();
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::SetPosition(float3 position)
{
	this->position = position;
	UpdateGlobalMatrix();
}

float3 ComponentTransform::GetGlobalPosition() const
{
	if (this->GetGameObject()->IsRoot())
	{
		return position;
	}
	else
	{
		ComponentTransform* parent_transform = (ComponentTransform*)GetGameObject()->GetParent()->GetComponent(ComponentType::Transform);
		return (position + parent_transform->position);
	}
}

float3 ComponentTransform::GetLocalPosition() const
{
		return position;
}

void ComponentTransform::SetRotation(float3 rotation)
{
	this->shown_rotation = rotation;
	this->rotation = Quat::FromEulerXYZ(rotation.x * DEGTORAD, rotation.y * DEGTORAD, rotation.z * DEGTORAD);
	UpdateGlobalMatrix();
}

float3 ComponentTransform::GetGlobalRotation() const
{
	if (GetGameObject()->GetParent() != nullptr)
	{
		ComponentTransform* parent_transform = (ComponentTransform*)GetGameObject()->GetParent()->GetComponent(ComponentType::Transform);
		return GetLocalRotation() + parent_transform->GetLocalRotation();
	}
	return GetLocalRotation(); //If it's the parent. local rotation = global rotation
}

float3 ComponentTransform::GetLocalRotation() const
{
	return shown_rotation;
}

void ComponentTransform::SetScale(float3 scale)
{
	this->scale = scale;
	UpdateGlobalMatrix();
}

float3 ComponentTransform::GetGlobalScale() const
{
	if (this->GetGameObject()->IsRoot())
	{
		return scale;
	}
	else
	{
		ComponentTransform* parent_transform = (ComponentTransform*)GetGameObject()->GetParent()->GetComponent(ComponentType::Transform);
		return (scale + parent_transform->scale);
	}
}

float3 ComponentTransform::GetLocalScale() const
{
	return scale;
}

void ComponentTransform::UpdateGlobalMatrix()
{
	//transform_matrix.SetIdentity();
	//transform_matrix.Scale(scale);
	//transform_matrix.RotateX(rotation.x);
	//transform_matrix.RotateY(rotation.y);
	//transform_matrix.RotateZ(rotation.z);
	//transform_matrix.Translate(position);
	if (!this->GetGameObject()->IsRoot())
	{
		ComponentTransform* parent_transform = (ComponentTransform*)this->GetGameObject()->GetParent()->GetComponent(Component::Transform);
		/*if(parent_transform->rotation.Length() != 0)
			transform_matrix = float4x4::FromTRS((position + parent_transform->position), parent_transform->rotation * rotation, scale + parent_transform->scale);
		else 
			transform_matrix = float4x4::FromTRS((position + parent_transform->position), Quat::identity * rotation, scale + parent_transform->scale);*/
		if (parent_transform->rotation.Length() != 0)
		{
			transform_matrix = float4x4::FromTRS(position, rotation, scale);
		}
		else
		{
			transform_matrix = float4x4::FromTRS(position, Quat::identity, scale);
		}

		transform_matrix = parent_transform->transform_matrix * transform_matrix;
	}
	else
	{
		transform_matrix = float4x4::FromTRS(position, rotation, scale);
		for (std::list<GameObject*>::iterator it = this->GetGameObject()->childs.begin(); it != this->GetGameObject()->childs.end(); it++)
		{
			ComponentTransform* child_transform = (ComponentTransform*)(*it)->GetComponent(Component::Transform);
			child_transform->UpdateGlobalMatrix();
		}
	}

	GetGameObject()->UpdateBoundingBox();
	GetGameObject()->UpdateCamera();
}

const float4x4 ComponentTransform::GetMatrix() const
{
	return transform_matrix;
}

const float * ComponentTransform::GetOpenGLMatrix() const
{
	return transform_matrix.Transposed().ptr();
}

void ComponentTransform::Save(Data & data) const
{
}

void ComponentTransform::Load(Data & data)
{
}
