#pragma once

#include "Window.h"

class GameObject;

class HierarchyWindow :
	public Window
{
public:
	HierarchyWindow();
	virtual ~HierarchyWindow();

	void DrawWindow();
	void DrawSceneGameObjects(GameObject* gameObject);
	//void CheckMouseOver(GameObject* gameObject);

private:
	bool show_rename_window;
	char node_name[30];
	bool show_rename_error;
	//GameObject* open_gameobject_node = nullptr;
	GameObject* gameobject_to_rename;
	float rename_window_y;
};

