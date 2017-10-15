#pragma once

#include "Window.h"

class HardwareWindow :
	public Window
{
public:
	HardwareWindow();
	virtual ~HardwareWindow();

	void DrawWindow();
};

