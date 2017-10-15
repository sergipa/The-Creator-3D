#pragma once

#include "Window.h"

class AppWindowConfigWindow : public Window
{
public:
	AppWindowConfigWindow();
	virtual ~AppWindowConfigWindow();
	
	void DrawWindow();

private:
	float fps_cap;
};