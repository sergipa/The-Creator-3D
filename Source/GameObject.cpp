#include "GameObject.h"
#include "ComponentMeshRenderer.h"

GameObject::GameObject()
{
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
		//componentsList.push_back(component = new ComponentTransform(this));
		break;
	case Component::Camera:
		break;
	case Component::RigidBody:
		break;
	case Component::MeshRenderer:
		componentsList.push_back(component = new ComponentMeshRenderer(this));
		break;
	case Component::BoxCollider:
		break;
	case Component::CircleCollider:
		break;
	case Component::AudioSource:
		//componentsList.push_back(component = new ComponentAudio(this));
		break;
	case Component::Animaton:
		//componentsList.push_back(component = new ComponentAnimation(this));
		break;
	case Component::Script:
		//componentsList.push_back(component = new ComponentScript(this));
		break;
	case Component::ParticleSystem:
		//componentsList.push_back(component = new ComponentParticle(this));
		break;
	default:
		break;
	}

	return component;
}

Component * GameObject::GetComponent(Component::ComponentType component_type)
{
	for (std::list<Component*>::iterator it = componentsList.begin(); it != componentsList.end(); it++) {
		if ((*it)->GetType() == component_type) {
			return (*it);
		}
	}
	return nullptr;
}

Component * GameObject::GetComponent(std::string component_type)
{
	for (std::list<Component*>::iterator it = componentsList.begin(); it != componentsList.end(); it++) {
		if ((*it)->GetName() == component_type) {
			return (*it);
		}
	}
	return nullptr;
}
