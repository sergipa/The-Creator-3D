#pragma once

#include "Window.h"

class HierarchyWindow :
	public Window
{
public:
	HierarchyWindow();
	virtual ~HierarchyWindow();

	void DrawWindow();
	//void DrawChilds(GameObject* gameObject);
	//void CheckMouseOver(GameObject* gameObject);

private:
	bool show_rename_window = false;
	char node_name[30];
	bool show_rename_error = false;
	//GameObject* open_gameobject_node = nullptr;
	//GameObject* gameobject_to_rename = nullptr;
	float rename_window_y;
};

