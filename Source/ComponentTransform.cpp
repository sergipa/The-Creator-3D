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

	if (!this->GetGameObject()->IsRoot())
	{
		ComponentTransform* parent_transform = (ComponentTransform*)this->GetGameObject()->GetParent()->GetComponent(Component::Transform);

		transform_matrix = transform_matrix.FromTRS(position, rotation, scale);
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
	//If gameobject has a camera component
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

void ComponentTransform::SetMatrix(const float4x4 & matrix)
{
	transform_matrix = matrix;

	//transform_matrix.Decompose(position, rotation, scale);
	//
	//float3 position, scale,float_rot;
	//Quat rotation;
	//matrix.Decompose(position, rotation, scale);
	//
	//float_rot = rotation.ToEulerXYZ();
	//SetPosition(position);
	//shown_rotation = float_rot;
	//SetRotation(shown_rotation);
	//SetScale(scale);

	//this->position = position;
	//this->rotation = rotation;
	//this->scale = scale;



	if (this->GetGameObject()->IsRoot())
	{
		for (std::list<GameObject*>::iterator it = this->GetGameObject()->childs.begin(); it != this->GetGameObject()->childs.end(); it++)
		{
			ComponentTransform* child_transform = (ComponentTransform*)(*it)->GetComponent(Component::Transform);
			child_transform->UpdateGlobalMatrix();
		}
	}
}

void ComponentTransform::Save(Data & data) const
{
	data.AddInt("Type", GetType());
	data.AddBool("Active", IsActive());
	data.AddUInt("UUID", GetUID());
	data.AddVector3("position", position);
	float4 quat_to_float;
	quat_to_float.x = rotation.x;
	quat_to_float.y = rotation.y;
	quat_to_float.z = rotation.z;
	quat_to_float.w = rotation.w;
	data.AddVector4("rotation", quat_to_float);
	data.AddVector3("scale", scale);
}

void ComponentTransform::Load(Data & data)
{
	SetType((Component::ComponentType)data.GetInt("Type"));
	SetActive(data.GetBool("Active"));
	SetUID(data.GetUInt("UUID"));
	position = data.GetVector3("position"); 
	float4 float_to_quat = data.GetVector4("rotation");
	rotation.x = float_to_quat.x;
	rotation.y = float_to_quat.y;
	rotation.z = float_to_quat.z;
	rotation.w = float_to_quat.w;
	shown_rotation = rotation.ToEulerXYZ();
	shown_rotation *= RADTODEG;
	scale = data.GetVector3("scale");
}
