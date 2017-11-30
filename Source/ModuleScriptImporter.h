#pragma once
#include "Module.h"
#include "mono/include/jit/jit.h"
#include "mono/include/metadata/assembly.h"
#include <vector>

class Script;
class CSScript;

class ModuleScriptImporter :
	public Module
{
public:
	ModuleScriptImporter(Application* app, bool start_enabled = true, bool is_game = false);
	~ModuleScriptImporter();

	bool Init(Data* editor_config = nullptr);

	//Retuns the library path if created or an empty string
	std::string ImportScript(std::string path);
	Script* LoadScriptFromLibrary(std::string path);

	MonoDomain* GetDomain() const;
	MonoImage* GetImage() const;

	int CompileScript(std::string assets_path, std::string library_path);

private:
	CSScript* DumpAssemblyInfo(MonoAssembly* assembly);
	MonoClass* DumpClassInfo(MonoImage* image, std::string& class_name, std::string& name_space);

private:
	std::string mono_path;
	MonoDomain* mono_domain;
	MonoImage* mono_image;
};

