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
	bool IsWindowFocused() const;
	bool IsMouseHoveringWindow() const;

private:
	float scene_width;
	float scene_height;
	bool wireframe_mode;
	bool is_window_focused;
	bool is_mouse_hovering_window;
};

