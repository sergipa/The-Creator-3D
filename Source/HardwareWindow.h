#pragma once

#include "Window.h"

class HardwareWindow :
	public Window
{
public:
	HardwareWindow();
	~HardwareWindow();

	void DrawWindow();
};

