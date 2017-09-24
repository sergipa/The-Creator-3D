#pragma once
#include "Window.h"
#include <experimental\filesystem>
namespace fs = std::experimental::filesystem;

struct SDL_Texture;

class AssetsWindow :
	public Window
{
public:
public:
	enum ScripType {
		cppScript, hScript, csScript, jsScript, luaScript, unkwnow
	};

	AssetsWindow();
	virtual ~AssetsWindow();

	void DrawWindow();

private:
	void FillAssetsLists();
	void DrawChilds(fs::path path);
	//void CreateScript(ScripType type, std::string scriptName);
	int String2int(std::string str);

private:
	fs::path selected_folder;
	uint node = 0;
	char node_name[30];
	bool show_new_folder_window = false;
	SDL_Texture* folder_image = nullptr;
	SDL_Texture* sound_image = nullptr;
	SDL_Texture* texture_image = nullptr;
	SDL_Texture* lua_script_image = nullptr;
	std::string file_panel;
	ImVec4 color;
	SDL_Texture* tool_tip_texture;
	fs::path assets_path;
	fs::path selected_file_path;
	bool file_options_open = false;
	bool show_new_script_window = false;
	ScripType script_type = unkwnow;
};

