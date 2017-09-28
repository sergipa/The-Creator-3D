#pragma once

#include "Module.h"
#include "Window.h"
#include <vector>

union SDL_Event;
struct SDL_Texture;

class HierarchyWindow;
class PropertiesWindow;
class AssetsWindow;
class ConsoleWindow;
class SceneWindow;
class HardwareWindow;
class PerformanceWindow;
class AboutWindow;
class AppWindowConfigWindow;
class EditorStyleWindow;
class RendererConfigWindow;

struct DragObjectData {
	std::string from_window;
	std::string path;
	std::string extension;
	std::string name;
	bool has_data = false;
	SDL_Texture* drag_sprite;
	void clearData() {
		from_window.clear();
		path.clear();
		extension.clear();
		name.clear();
		has_data = false;
		drag_sprite = nullptr;
	}
};

class ModuleEditor :
	public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init(Data* editor_config = nullptr);
	update_status PreUpdate(float delta_time);
	update_status Update(float dt);
	bool DrawEditor();
	bool CleanUp();

	void HandleInput(SDL_Event* event);
	void OpenBrowserPage(const char* url);

	void AddData_Editor(float ms, float fps);

	void LoadEditorStyle();

private:
	ImFont* font = nullptr;
	std::vector<Window*> editor_windows;
	HierarchyWindow* hierarchy_window = nullptr;
	AssetsWindow* assets_window = nullptr;
	PropertiesWindow* properties_window = nullptr;
	SceneWindow* scene_window = nullptr;
	ConsoleWindow* console_window = nullptr;
	HardwareWindow* hardware_window = nullptr;
	PerformanceWindow* performance_window = nullptr;
	AboutWindow* about_window = nullptr;
	AppWindowConfigWindow* config_window = nullptr;
	EditorStyleWindow* style_editor_window = nullptr;
	RendererConfigWindow* renderer_config_window = nullptr;
	//AnimatorWindow* animator_window = nullptr;
	//ParticleEditorWindow* particle_editor_window = nullptr;

public:
	DragObjectData drag_data;
	//GameObject* dragging_gameobject = nullptr;
};

