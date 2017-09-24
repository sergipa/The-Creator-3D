#pragma once

#include "Window.h"

class SceneWindow :
	public Window
{
public:
	SceneWindow();
	virtual ~SceneWindow();

	void DrawWindow();

private:
	float scene_width;
	float scene_height;
};

