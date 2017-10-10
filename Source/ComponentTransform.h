#pragma once
#include "Component.h"
#include "MathGeoLib\Math\float3.h"

class ComponentTransform :
	public Component
{
public:
	ComponentTransform(GameObject* attached_gameobject);
	~ComponentTransform();

	void SetPosition(float3 position);
	float3 GetGlobalPosition() const;
	float3 GetLocalPosition() const;
	void SetRotation(float3 rotation);
	float3 GetGlobalRotation() const;
	float3 GetLocalRotation() const;
	void SetScale(float3 scale);
	float3 GetGlobalScale() const;
	float3 GetLocalScale() const;

	void Save(Data& data) const;
	void Load(Data& data);

private:
	float3 position;
	float3 rotation;
	float3 scale;
};

