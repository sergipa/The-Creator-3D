#pragma once
#include "Component.h"
#include "glmath.h"

class ComponentTransform :
	public Component
{
public:
	ComponentTransform(GameObject* attached_gameobject);
	~ComponentTransform();

	void SetPosition(vec3 position);
	vec3 GetGlobalPosition() const;
	vec3 GetLocalPosition() const;
	void SetRotation(vec3 rotation);
	vec3 GetGlobalRotation() const;
	vec3 GetLocalRotation() const;
	void SetScale(vec3 scale);
	vec3 GetGlobalScale() const;
	vec3 GetLocalScale() const;

	void Save(Data& data) const;
	void Load(Data& data);

private:
	vec3 position;
	vec3 rotation;
	vec3 scale;
};

