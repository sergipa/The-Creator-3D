#include "ComponentTransform.h"
#include "GameObject.h"

ComponentTransform::ComponentTransform(GameObject* attached_gameobject)
{
	SetActive(true);
	SetName("Transform");
	SetType(ComponentType::Transform);
	SetGameObject(attached_gameobject);

	position = vec3(0,0,0);
	rotation = vec3(0,0,0);
	scale = vec3(1,1,1);
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::SetPosition(vec3 position)
{
	this->position = position;
}

vec3 ComponentTransform::GetGlobalPosition() const
{
	return position;
}

vec3 ComponentTransform::GetLocalPosition() const
{
	if (GetGameObject()->GetParent() != nullptr)
	{
		ComponentTransform* parent_transform = (ComponentTransform*)GetGameObject()->GetParent()->GetComponent(ComponentType::Transform);
		return GetGlobalPosition() - parent_transform->GetGlobalPosition();
	}
	return GetGlobalPosition(); //If it's the parent. local position = global position
}

void ComponentTransform::SetRotation(vec3 rotation)
{
	this->rotation = rotation;
}

vec3 ComponentTransform::GetGlobalRotation() const
{
	return rotation;
}

vec3 ComponentTransform::GetLocalRotation() const
{
	if (GetGameObject()->GetParent() != nullptr)
	{
		ComponentTransform* parent_transform = (ComponentTransform*)GetGameObject()->GetParent()->GetComponent(ComponentType::Transform);
		return GetGlobalRotation() - parent_transform->GetGlobalRotation();
	}
	return GetGlobalRotation(); //If it's the parent. local rotation = global rotation
}

void ComponentTransform::SetScale(vec3 scale)
{
	this->scale = scale;
}

vec3 ComponentTransform::GetGlobalScale() const
{
	return scale;
}

vec3 ComponentTransform::GetLocalScale() const
{
	if (GetGameObject()->GetParent() != nullptr)
	{
		ComponentTransform* parent_transform = (ComponentTransform*)GetGameObject()->GetParent()->GetComponent(ComponentType::Transform);
		return (GetGlobalScale() - parent_transform->GetGlobalScale()) + vec3(1,1,1); //scale is 1,1,1 if not modified
	}
	return GetGlobalScale(); //If it's the parent. local scale = global scale
}

void ComponentTransform::Save(Data & data) const
{
}

void ComponentTransform::Load(Data & data)
{
}
