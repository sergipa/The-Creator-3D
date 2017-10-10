#pragma once

#include "Data.h"
#include "Component.h"
#include <list>
#include <string>

class GameObject
{
public:
	GameObject(GameObject* parent = nullptr);
	~GameObject();

	Component* AddComponent(Component::ComponentType component_type);
	Component* GetComponent(Component::ComponentType component_type);
	Component* GetComponent(std::string component_type);

	void AddChild(GameObject* gameobject);
	bool IsActive() const;
	void SetActive(bool active);
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

	void Destroy();

public:
	std::list<GameObject*> childs;
	std::list<Component*> components_list;
private:
	bool active;
	GameObject* parent;
	std::string name;
	std::string tag;
	std::string layer;
	bool is_root;
};

