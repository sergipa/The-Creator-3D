#pragma once
#include "Module.h"

class Texture;

class ModuleTextureImporter :
	public Module
{
public:
	ModuleTextureImporter(Application* app, bool start_enabled = true, bool is_game = false);
	~ModuleTextureImporter();

	bool Init(Data* editor_config = nullptr);
	bool CleanUp();

	bool ImportTexture(std::string path);
	Texture* LoadTextureFromLibrary(std::string path);
};

