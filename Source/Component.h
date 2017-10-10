#pragma once

#include "Data.h"
#include <string>

class GameObject;

class Component
{
public:
	enum ComponentType {
		Transform, Camera, RigidBody, MeshRenderer, BoxCollider, CircleCollider, AudioSource, Animaton, Script, ParticleSystem, Unknown
	};

	Component();
	virtual ~Component();
	virtual void OnEnable();
	virtual void OnDisable();
	void SetActive(bool active);
	bool IsActive() const;
	void SetGameObject(GameObject* attached_gameobject);
	GameObject* GetGameObject() const;
	void SetType(ComponentType type);
	ComponentType GetType() const;
	void SetName(std::string name);
	std::string GetName() const;

	virtual void Save(Data& data) const = 0;
	virtual void Load(Data& data) = 0;

	

private:
	ComponentType type;
	GameObject* gameobject;
	std::string name;
	bool active;
};

