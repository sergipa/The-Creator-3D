#pragma once

#include "Window.h"

class AppWindowConfigWindow : public Window
{
public:
	AppWindowConfigWindow();
	~AppWindowConfigWindow();
	void DrawWindow();

private:
	float fps_cap;
};