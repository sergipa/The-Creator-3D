#pragma once
#include "Module.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
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

	int CompileScript(std::string assets_path);

private:
	CSScript* DumpAssemblyInfo(MonoAssembly* assembly);
	MonoClass* DumpClassInfo(MonoImage* image, std::string& class_name, std::string& name_space);

	void RegisterAPI();

	static void SetGameObjectName(MonoObject * object, MonoString* name);
	static void CreateGameObject(MonoObject * object, MonoObject * object2);
	static void SetSelf();
	static void Log(MonoObject* object);
	static void Warning(MonoObject* object);
	static void Error(MonoObject* object);

private:
	std::string mono_path;
	MonoDomain* mono_domain;
	MonoImage* mono_image;
	static CSScript* current_script;
	static bool inside_function;
};

