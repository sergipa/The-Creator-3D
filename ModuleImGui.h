#pragma once
#include "Module.h"
#include "Globals.h"

class Console;

class ModuleImGui : public Module
{
public:
	ModuleImGui(Application* app, bool start_enabled = true);
	~ModuleImGui();

	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void AddLogToConsole(const char* log);

private:
	bool show_test_window;
	bool show_console;

public:
	Console* console = nullptr;
};