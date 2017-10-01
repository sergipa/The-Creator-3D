#pragma once

#include "Window.h"

class SceneWindow :
	public Window
{
public:
	SceneWindow();
	virtual ~SceneWindow();

	void DrawWindow();
	void DrawMenuBar();
	void HandleInput();

private:
	float scene_width;
	float scene_height;
	bool wireframe_mode;
};

