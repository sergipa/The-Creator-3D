#pragma once

#include "Window.h"
#include <map>
#include "ComponentMeshRenderer.h"
#include "ComponentTransform.h"

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

};

