#pragma once

#include "Window.h"
#include <map>

class ComponentMeshRenderer;
class ComponentTransform;
class ComponentCamera;
class Component;

class PropertiesWindow :
	public Window
{
public:
	PropertiesWindow();
	virtual ~PropertiesWindow();

	void DrawWindow();
	void DrawComponent(Component* component);
	void DrawTransformPanel(ComponentTransform* transform);
	void DrawMeshRendererPanel(ComponentMeshRenderer* spriteRenderer);
	void DrawCameraPanel(ComponentCamera* camera);
};

