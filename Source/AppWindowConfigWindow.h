#pragma once
#include "Window.h"
class AppWindowConfigWindow : public Window
{
public:
	AppWindowConfigWindow();
	~AppWindowConfigWindow();
	void DrawWindow();
private:
	bool fullscreen;
	//bool resizable;
	bool fullscren_desktop;

	float brightness;
	float width;
	float height;
	float fps_cap;
};