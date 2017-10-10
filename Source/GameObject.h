#pragma once

#include "Data.h"
#include "Component.h"
#include <list>
#include <string>

class GameObject
{
public:
	GameObject();
	~GameObject();

	Component* AddComponent(Component::ComponentType component_type);
	Component* GetComponent(Component::ComponentType component_type);
	Component* GetComponent(std::string component_type);

private:
	std::list<Component*> componentsList;
};

