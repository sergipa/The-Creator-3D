#pragma once

#include <string>
#include "Globals.h"
#include "imgui\imgui.h"
#include "imgui\imguidock.h"

class Window
{
public:
	Window();
	virtual ~Window();

	void SetActive(bool active);
	bool IsActive() const;
	virtual void DrawWindow() = 0;

public:
	bool active;
	std::string window_name;
};

