#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init(Data* editor_config = nullptr);
	bool CleanUp();

	void SetTitle(const char* title);
	uint GetWidth() const;
	uint GetHeight() const;
	void SetWidth(uint width);
	void SetHeight(uint height);
	void SetIcon(const char* path);

	void SaveData(Data* data);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	SDL_Texture* engine_icon;

private:
	uint screen_width;
	uint screen_height;
	bool is_fullscreen;
	bool is_resizable;
	bool is_borderless;
	bool is_full_desktop;
};

#endif // __ModuleWindow_H__