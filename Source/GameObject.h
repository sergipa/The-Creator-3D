#pragma once

#include "Data.h"
#include "Component.h"
#include <list>
#include <string>
#include "MathGeoLib\Math\float4x4.h"

class GameObject
{
public:
	GameObject(GameObject* parent = nullptr);
	~GameObject();

	Component* AddComponent(Component::ComponentType component_type);
	Component* GetComponent(Component::ComponentType component_type);
	Component* GetComponent(std::string component_type);
	void GameObject::DestroyComponent(Component::ComponentType component);

	void AddChild(GameObject* gameobject);
	bool IsActive() const;
	void SetActive(bool active);
	void SetStatic(bool is_static);
	bool IsStatic() const;
	bool IsSelected() const;
	void SetSelected(bool selected);
	bool IsRoot() const;
	void SetRoot(bool root);
	GameObject* GetParent() const;
	void SetParent(GameObject* parent);
	void SetName(std::string name);
	std::string GetName() const;
	void SetTag(std::string tag);
	std::string GetTag() const;
	void SetLayer(std::string layer);
	std::string GetLayer() const;

	void UpdateBoundingBox();
	math::float4x4 GetGlobalTransfomMatrix();
	const float* GetOpenGLMatrix();
	void UpdateGlobalMatrix();
	void UpdateCamera();

	void Destroy();
	void OnDestroy();

	UID GetUID() const;

public:
	std::list<GameObject*> childs;
	std::list<Component*> components_list;
	bool is_on_destroy;

private:
	bool active;
	GameObject* parent;
	std::string name;
	std::string tag;
	std::string layer;
	bool is_root;
	UID uuid;
	bool is_selected;
	bool is_static;
};

