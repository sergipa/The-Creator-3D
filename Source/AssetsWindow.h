#pragma once
#include "Window.h"
#include <filesystem>
namespace fs = std::experimental::filesystem;

//C++ filesystem library: http://en.cppreference.com/w/cpp/experimental/fs

class Texture;

class AssetsWindow :
	public Window
{

public:

	AssetsWindow();
	virtual ~AssetsWindow();

	void DrawWindow();

private:
	void FillAssetsLists();
	void DrawChilds(fs::path path);
	int ExtensionToResourceType(std::string str);

private:
	uint node;
	char node_name[30];
	bool show_new_folder_window;
	bool file_options_open;

	Texture* texture_icon;
	Texture* mesh_icon;
	Texture* font_icon;
	Texture* folder_icon;

	fs::path selected_file_path;
	fs::path selected_folder;
};

