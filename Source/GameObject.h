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
	GameObject* GetParent() const;
	void SetName(std::string name);
	std::string GetName() const;
	void Destroy();

public:
	std::list<GameObject*> childs;

private:
	std::list<Component*> componentsList;
	bool active;
	GameObject* parent;
	std::string name;
};

