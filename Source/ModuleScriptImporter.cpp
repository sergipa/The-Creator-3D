#include "ModuleScriptImporter.h"
#include "Script.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include <mono/utils/mono-logger.h>
#include <mono/metadata/metadata.h>
#include "CSScript.h"
#include "GameObject.h"
#include "ModuleScene.h"

CSScript* ModuleScriptImporter::current_script = nullptr;
bool ModuleScriptImporter::inside_function = false;

ModuleScriptImporter::ModuleScriptImporter(Application* app, bool start_enabled, bool is_game) : Module(app, start_enabled, is_game)
{
	name = "Script_Importer";
	mono_domain = nullptr;
	mono_engine_image = nullptr;
}

ModuleScriptImporter::~ModuleScriptImporter()
{
}

void MonoInternalWarning(const char * string, mono_bool is_stdout)
{
	CONSOLE_WARNING("%s", string);
}

void MonoInternalError(const char * string, mono_bool is_stdout)
{
	CONSOLE_ERROR("%s", string);
}

void MonoLogToLog(const char * log_domain, const char * log_level, const char * message, mono_bool fatal, void * user_data)
{
	if (fatal || log_level == "error")
	{
		CONSOLE_ERROR("%s%s", log_domain != nullptr ? ((std::string)log_domain + ": ").c_str() : "", message);
	}
	else if (log_level == "warning")
	{
		CONSOLE_WARNING("%s%s", log_domain != nullptr ? ((std::string)log_domain + ": ").c_str() : "", message);
	}
	else if (log_level == "critical")
	{
		CONSOLE_ERROR("%s%s", log_domain != nullptr ? ((std::string)log_domain + ": ").c_str() : "", message);
	}
	else
	{
		CONSOLE_LOG("%s%s", log_domain != nullptr ? ((std::string)log_domain + ": ").c_str() : "", message);
	}
}

bool ModuleScriptImporter::Init(Data * editor_config)
{
	mono_trace_set_log_handler(MonoLogToLog, this);
	mono_trace_set_print_handler(MonoInternalWarning);
	mono_trace_set_printerr_handler(MonoInternalError);

	mono_path = App->file_system->GetFullPath("mono/");

	mono_set_dirs((mono_path + "lib").c_str(), (mono_path + "etc").c_str());

	mono_domain = mono_jit_init("TheCreator");

	MonoAssembly* engine_assembly = mono_domain_assembly_open(mono_domain, REFERENCE_ASSEMBLIES_FOLDER"TheEngine.dll");
	if (engine_assembly)
	{
		mono_engine_image = mono_assembly_get_image(engine_assembly);
		RegisterAPI();
	}
	else
	{
		CONSOLE_ERROR("Can't load 'TheEngine.dll'!");
		return false;
	}

	return true;
}

std::string ModuleScriptImporter::ImportScript(std::string path)
{
	std::string ret = "";
	std::string script_name = App->file_system->GetFileNameWithoutExtension(path);
	
	if (CompileScript(path) != 0)
	{
		CONSOLE_ERROR("Can't compile %s", path.c_str());
	}
	else
	{
		if (App->file_system->FileExist(TMP_FOLDER + script_name + ".dll"))
		{
			if (App->file_system->Copy_File(TMP_FOLDER + script_name + ".dll", LIBRARY_SCRIPTS_FOLDER + script_name + ".dll"))
			{
				App->file_system->Delete_File(TMP_FOLDER + script_name + ".dll");
				ret = LIBRARY_SCRIPTS_FOLDER + script_name + ".dll";
			}
		}
	}

	return ret;
}

Script * ModuleScriptImporter::LoadScriptFromLibrary(std::string path)
{
	MonoAssembly* assembly = mono_domain_assembly_open(mono_domain, path.c_str());
	if (assembly)
	{
		CSScript* script = DumpAssemblyInfo(assembly);
		if (script != nullptr)
		{
			if (script->LoadScript(path))
			{
				return script;
			}
		}
	}

	return nullptr;
}

MonoDomain * ModuleScriptImporter::GetDomain() const
{
	return mono_domain;
}

MonoImage * ModuleScriptImporter::GetEngineImage() const
{
	return mono_engine_image;
}

int ModuleScriptImporter::CompileScript(std::string assets_path)
{
	if (!App->file_system->DirectoryExist(TMP_FOLDER_PATH)) App->file_system->Create_Directory(TMP_FOLDER_PATH);
	std::string script_name = App->file_system->GetFileNameWithoutExtension(assets_path);
	std::string compile_command = mono_path + "compiler\\mcs -debug -target:library -out:" + TMP_FOLDER + script_name + ".dll" + " ";
	std::string assemblies_folder_full_path = App->file_system->GetFullPath("Editor_Settings/Reference_Assemblies/");
	std::vector<std::string> assemblies = App->file_system->GetFilesInDirectory(assemblies_folder_full_path);
	for (std::vector<std::string>::iterator it = assemblies.begin(); it != assemblies.end(); it++)
	{
		if (App->file_system->GetFileExtension(*it) == ".dll")
		{
			compile_command += "-r:" + assemblies_folder_full_path + App->file_system->GetFileName(*it) + " ";
		}
	}
	for (std::vector<std::string>::iterator it = assemblies.begin(); it != assemblies.end(); it++)
	{
		if (App->file_system->GetFileExtension(*it) == ".dll")
		{
			compile_command += "-lib:" + assemblies_folder_full_path + App->file_system->GetFileName(*it) + " ";
		}
	}

	std::string d = App->file_system->GetFullPath(assets_path);
	compile_command += assets_path;
	return system(compile_command.c_str());
}

void ModuleScriptImporter::SetCurrentScript(CSScript * script)
{
	current_script = script;
}

CSScript* ModuleScriptImporter::DumpAssemblyInfo(MonoAssembly * assembly)
{
	MonoImage* mono_image = mono_assembly_get_image(assembly);
	if (mono_image)
	{
		std::string class_name;
		std::string name_space;
		MonoClass* loaded_script = DumpClassInfo(mono_image, class_name, name_space);

		if (loaded_script != nullptr)
		{
			CSScript* script = new CSScript();
			script->SetDomain(mono_domain);
			script->SetImage(mono_image);
			script->SetName(class_name);
			script->SetClassName(class_name);
			script->SetNameSpace(name_space);
			script->SetClass(loaded_script);
			return script;
		}
	}
}

MonoClass* ModuleScriptImporter::DumpClassInfo(MonoImage * image, std::string& class_name, std::string& name_space)
{
	MonoClass* mono_class = nullptr;

	const MonoTableInfo* table_info = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);

	int rows = mono_table_info_get_rows(table_info);

	for (int i = 1; i < rows; i++) {
		uint32_t cols[MONO_TYPEDEF_SIZE];
		mono_metadata_decode_row(table_info, i, cols, MONO_TYPEDEF_SIZE);
		const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
		const char* _name_space = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
		mono_class = mono_class_from_name(image, _name_space, name);
		if (mono_class)
		{
			class_name = name;
			name_space = _name_space;
		}
	}

	return mono_class;
}

void ModuleScriptImporter::RegisterAPI()
{
	//GAMEOBJECT
	mono_add_internal_call("TheEngine.TheGameObject::SetName", (const void*)SetGameObjectName);
	mono_add_internal_call("TheEngine.TheGameObject::GetName", (const void*)GetGameObjectName);
	mono_add_internal_call("TheEngine.TheGameObject::SetActive", (const void*)SetGameObjectActive);
	mono_add_internal_call("TheEngine.TheGameObject::IsActive", (const void*)GetGameObjectIsActive);
	mono_add_internal_call("TheEngine.TheGameObject::CreateNewGameObject", (const void*)CreateGameObject);
	mono_add_internal_call("TheEngine.TheGameObject::GetSelf", (const void*)SetSelfGameObject);
	mono_add_internal_call("TheEngine.TheGameObject::AddComponent", (const void*)AddComponent);
	mono_add_internal_call("TheEngine.TheGameObject::GetComponent", (const void*)GetComponent);

	//TRANSFORM
	mono_add_internal_call("TheEngine.TheTransform::SetPosition", (const void*)SetPosition);
	mono_add_internal_call("TheEngine.TheTransform::GetPosition", (const void*)GetPosition);

	//INPUT
	mono_add_internal_call("TheEngine.TheInput::IsKeyDown", (const void*)IsKeyDown);
	mono_add_internal_call("TheEngine.TheInput::IsKeyUp", (const void*)IsKeyUp);
	mono_add_internal_call("TheEngine.TheInput::IsKeyRepeat", (const void*)IsKeyRepeat);
	mono_add_internal_call("TheEngine.TheInput::IsMouseButtonDown", (const void*)IsMouseDown);
	mono_add_internal_call("TheEngine.TheInput::IsMouseButtonUp", (const void*)IsMouseUp);
	mono_add_internal_call("TheEngine.TheInput::IsMouseButtonRepeat", (const void*)IsMouseRepeat);

	//CONSOLE
	mono_add_internal_call("TheEngine.TheConsole.TheConsole::Log", (const void*)Log);
	mono_add_internal_call("TheEngine.TheConsole.TheConsole::Warning", (const void*)Warning);
	mono_add_internal_call("TheEngine.TheConsole.TheConsole::Error", (const void*)Error);
}

void ModuleScriptImporter::SetGameObjectName(MonoObject * object, MonoString * name, MonoObject * object2)
{
	current_script->SetGameObjectName(object, name);
}

MonoString* ModuleScriptImporter::GetGameObjectName(MonoObject * object)
{
	return current_script->GetGameObjectName(object);
}

void ModuleScriptImporter::SetGameObjectActive(MonoObject * object, mono_bool active)
{
	current_script->SetGameObjectActive(object, active);
}

mono_bool ModuleScriptImporter::GetGameObjectIsActive(MonoObject * object)
{
	return current_script->GetGameObjectActive(object);
}

void ModuleScriptImporter::CreateGameObject(MonoObject * object)
{
	current_script->CreateGameObject(object);
}

MonoObject* ModuleScriptImporter::SetSelfGameObject()
{
	return current_script->SetSelfGameObject();
}

void ModuleScriptImporter::SetGameObjectTag(MonoObject * object, MonoString * name)
{
	current_script->SetGameObjectTag(object, name);
}

MonoString* ModuleScriptImporter::GetGameObjectTag(MonoObject * object)
{
	return current_script->GetGameObjectTag(object);
}

void ModuleScriptImporter::SetGameObjectLayer(MonoObject * object, MonoString * layer)
{
	current_script->SetGameObjectLayer(object, layer);
}

MonoString * ModuleScriptImporter::GetGameObjectLayerName(MonoObject * object)
{
	return current_script->GetGameObjectLayer(object);
}

void ModuleScriptImporter::SetGameObjectStatic(MonoObject * object, mono_bool value)
{
	current_script->SetGameObjectStatic(object, value);
}

mono_bool ModuleScriptImporter::GameObjectIsStatic(MonoObject * object)
{
	return current_script->GameObjectIsStatic(object);
}

MonoObject* ModuleScriptImporter::AddComponent(MonoObject * object, MonoReflectionType* type)
{
	return current_script->AddComponent(object, type);
}

MonoObject* ModuleScriptImporter::GetComponent(MonoObject * object, MonoReflectionType * type)
{
	return current_script->GetComponent(object, type);
}

void ModuleScriptImporter::SetPosition(MonoObject * object, MonoObject * vector3)
{
	current_script->SetPosition(object, vector3);
}

MonoObject* ModuleScriptImporter::GetPosition(MonoObject * object)
{
	return current_script->GetPosition(object);
}

mono_bool ModuleScriptImporter::IsKeyDown(MonoString * key_name)
{
	return current_script->IsKeyDown(key_name);
}

mono_bool ModuleScriptImporter::IsKeyUp(MonoString * key_name)
{
	return current_script->IsKeyUp(key_name);
}

mono_bool ModuleScriptImporter::IsKeyRepeat(MonoString * key_name)
{
	return current_script->IsKeyRepeat(key_name);
}

mono_bool ModuleScriptImporter::IsMouseDown(int mouse_button)
{
	return current_script->IsMouseDown(mouse_button);
}

mono_bool ModuleScriptImporter::IsMouseUp(int mouse_button)
{
	return current_script->IsMouseUp(mouse_button);
}

mono_bool ModuleScriptImporter::IsMouseRepeat(int mouse_button)
{
	return current_script->IsMouseRepeat(mouse_button);
}

void ModuleScriptImporter::Log(MonoObject * object)
{
	MonoObject* exception = nullptr;
	MonoString* str = mono_object_to_string(object, &exception);
	if (exception)
	{
		mono_print_unhandled_exception(exception);
	}
	else
	{
		const char* message = mono_string_to_utf8(str);
		CONSOLE_LOG("%s", message);
	}
}

void ModuleScriptImporter::Warning(MonoObject * object)
{
	MonoObject* exception = nullptr;
	MonoString* str = mono_object_to_string(object, &exception);
	if (exception)
	{
		mono_print_unhandled_exception(exception);
	}
	else
	{
		const char* message = mono_string_to_utf8(str);
		CONSOLE_WARNING("%s", message);
	}
}

void ModuleScriptImporter::Error(MonoObject * object)
{
	MonoObject* exception = nullptr;
	MonoString* str2 = mono_object_to_string(object, &exception);
	if (exception)
	{
		mono_print_unhandled_exception(exception);
	}
	else
	{
		const char* message = mono_string_to_utf8(str2);
		CONSOLE_ERROR("%s", message);
	}
}
