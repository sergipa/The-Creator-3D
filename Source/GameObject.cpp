#include "GameObject.h"
#include "ComponentMeshRenderer.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ComponentTransform.h"

GameObject::GameObject(GameObject* parent)
{
	active = true;
	this->parent = nullptr;
	if (parent != nullptr)
	{
		SetParent(parent);
	}
	else
	{
		is_root = true;
	}
	tag = "None";
	layer = "None";
	name = "New GameObject";

	AddComponent(Component::Transform);
}

GameObject::~GameObject()
{
}

Component * GameObject::AddComponent(Component::ComponentType component_type)
{
	Component* component = nullptr;

	switch (component_type)
	{
	case Component::Transform:
		components_list.push_back(component = new ComponentTransform(this));
		break;
	case Component::Camera:
		break;
	case Component::RigidBody:
		break;
	case Component::MeshRenderer:
		components_list.push_back(component = new ComponentMeshRenderer(this));
		break;
	case Component::BoxCollider:
		break;
	case Component::CircleCollider:
		break;
	case Component::AudioSource:
		break;
	case Component::Animaton:
		break;
	case Component::Script:
		break;
	case Component::ParticleSystem:
		break;
	default:
		break;
	}

	return component;
}

Component * GameObject::GetComponent(Component::ComponentType component_type)
{
	for (std::list<Component*>::iterator it = components_list.begin(); it != components_list.end(); it++) {
		if ((*it)->GetType() == component_type) {
			return (*it);
		}
	}
	return nullptr;
}

Component * GameObject::GetComponent(std::string component_type)
{
	for (std::list<Component*>::iterator it = components_list.begin(); it != components_list.end(); it++) {
		if ((*it)->GetName() == component_type) {
			return (*it);
		}
	}
	return nullptr;
}

void GameObject::AddChild(GameObject * gameobject)
{
	childs.push_back(gameobject);
}

bool GameObject::IsActive() const
{
	return active;
}

void GameObject::SetActive(bool active)
{
	this->active = active;
}

bool GameObject::IsRoot() const
{
	return is_root;
}

void GameObject::SetRoot(bool root)
{
	is_root = root;
}

GameObject * GameObject::GetParent() const
{
	return parent;
}

void GameObject::SetParent(GameObject * parent)
{
	if (this->parent == parent) {
		return;
	}

	if (this->parent != nullptr) {
		this->parent->childs.remove(this);
	}

	this->parent = parent;
	this->parent->childs.push_back(this);
	is_root = false;
	App->scene->root_gameobjects.remove(this);
}

void GameObject::SetName(std::string name)
{
	this->name = name;
}

std::string GameObject::GetName() const
{
	return name;
}

void GameObject::SetTag(std::string tag)
{
	this->tag = tag;
}

std::string GameObject::GetTag() const
{
	return tag;
}

void GameObject::SetLayer(std::string layer)
{
	this->layer = layer;
}

std::string GameObject::GetLayer() const
{
	return layer;
}

void GameObject::Destroy()
{
	App->scene->AddGameObjectToDestroy(this);
}

