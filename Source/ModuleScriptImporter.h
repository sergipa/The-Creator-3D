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
	MonoImage* GetEngineImage() const;

	int CompileScript(std::string assets_path);
	void SetCurrentScript(CSScript* script);

private:
	CSScript* DumpAssemblyInfo(MonoAssembly* assembly);
	MonoClass* DumpClassInfo(MonoImage* image, std::string& class_name, std::string& name_space);

	void RegisterAPI();

	//GAMEOBJECT
	static void SetGameObjectName(MonoObject * object, MonoString* name, MonoObject * object2);
	static MonoString* GetGameObjectName(MonoObject* object);
	static void CreateGameObject(MonoObject * object);
	static MonoObject* SetSelfGameObject();
	static void SetGameObjectActive(MonoObject * object, mono_bool active);
	static mono_bool GetGameObjectIsActive(MonoObject* object);
	static void SetGameObjectTag(MonoObject * object, MonoString* tag);
	static MonoString* GetGameObjectTag(MonoObject* object);
	static void SetGameObjectLayer(MonoObject * object, MonoString* layer);
	static MonoString* GetGameObjectLayerName(MonoObject* object);
	static void SetGameObjectStatic(MonoObject * object, mono_bool value);
	static mono_bool GameObjectIsStatic(MonoObject* object);
	static MonoObject* AddComponent(MonoObject* object, MonoReflectionType* type);
	static MonoObject* GetComponent(MonoObject* object, MonoReflectionType* type);

	//TRANSFORM
	static void SetPosition(MonoObject * object, MonoObject * object2);
	static MonoObject* GetPosition(MonoObject* object);

	//INPUT
	static mono_bool IsKeyDown(MonoString * key_name);
	static mono_bool IsKeyUp(MonoString* key_name);
	static mono_bool IsKeyRepeat(MonoString* key_name);
	static mono_bool IsMouseDown(int mouse_button);
	static mono_bool IsMouseUp(int mouse_button);
	static mono_bool IsMouseRepeat(int mouse_button);

	//CONSOLE
	static void Log(MonoObject* object);
	static void Warning(MonoObject* object);
	static void Error(MonoObject* object);

private:
	std::string mono_path;
	MonoDomain* mono_domain;
	MonoImage* mono_engine_image;
	static CSScript* current_script;
	static bool inside_function;
};

