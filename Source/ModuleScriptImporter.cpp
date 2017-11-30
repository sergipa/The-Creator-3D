#include "ModuleScriptImporter.h"
#include "Script.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "mono/include/utils/mono-logger.h"
#include "mono/include/metadata/metadata.h"
#include "CSScript.h"

ModuleScriptImporter::ModuleScriptImporter(Application* app, bool start_enabled, bool is_game) : Module(app, start_enabled, is_game)
{
	name = "Script_Importer";
	mono_domain = nullptr;
	mono_image = nullptr;
}

ModuleScriptImporter::~ModuleScriptImporter()
{
}

void MonoInternalPrint(const char * string, mono_bool is_stdout)
{
	CONSOLE_WARNING("%s", string);
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
	mono_trace_set_log_handler(MonoLogToLog, nullptr);
	mono_trace_set_print_handler(MonoInternalPrint);
	mono_trace_set_printerr_handler(MonoInternalPrint);

	mono_path = App->file_system->GetFileDirectory(__FILE__) + "\\mono\\";

	mono_set_dirs((mono_path + "lib").c_str(), (mono_path + "etc").c_str());

	mono_domain = mono_jit_init("TheCreator");

	/*bool recompile_scripts = false;

	if (App->file_system->DirectoryExist(LIBRARY_SCRIPTS_FOLDER_PATH))
	{
		if (App->file_system->FileExist(LIBRARY_SCRIPTS_FOLDER"Assembly-CSharp.dll"))
		{
			MonoAssembly* assembly = mono_domain_assembly_open(mono_domain, LIBRARY_SCRIPTS_FOLDER"Assembly-CSharp.dll");
			if (assembly)
			{
				DumpAssemblyInfo(assembly);
			}
		}
	}*/

	return true;
}

std::string ModuleScriptImporter::ImportScript(std::string path)
{
	std::string ret = "";

	std::string library_folder = LIBRARY_SCRIPTS_FOLDER;
	std::string script_name = App->file_system->GetFileNameWithoutExtension(path);
	std::string library_path = library_folder + script_name + ".dll";
	
	if (CompileScript(path, library_path) != 0)
	{
		CONSOLE_ERROR("Can't compile %s", path.c_str());
	}
	else
	{
		ret = library_folder + script_name + ".dll";
	}

	/*std::string merge_command = mono_path + "bin\\ILMerge.exe -target:library -allowDup:.dll -out:" + library_folder + "Assembly-CSharp.dll ";
	
	std::vector<std::string> files = App->file_system->GetFilesInDirectory(LIBRARY_SCRIPTS_FOLDER_PATH);
	for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); it++)
	{
		if (App->file_system->GetFileExtension(*it) != ".dll") continue;
		merge_command += *it + " ";
	}*/

	//merge_command += library_folder + "*.dll -target:library";
	//system(merge_command.c_str());
	//App->file_system->Delete_File(ret);

	/*std::string script_name = App->file_system->GetFileNameWithoutExtension(path);

	for (std::vector<MonoClass*>::iterator it = loaded_scripts.begin(); it != loaded_scripts.end(); it++)
	{
		if (script_name != mono_class_get_name(*it)) compile_scripts = true;
	}

	ret = LIBRARY_SCRIPTS_FOLDER"Assembly-CSharp.dll";*/
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

MonoImage * ModuleScriptImporter::GetImage() const
{
	return mono_image;
}

int ModuleScriptImporter::CompileScript(std::string assets_path, std::string library_path)
{
	std::string compile_command = mono_path + "bin\\mcs -target:library " + assets_path + " -out:" + library_path + " ";
	std::string folder = REFERENCE_ASSEMBLIES_FOLDER;
	std::vector<std::string> assemblies = App->file_system->GetFilesInDirectory(folder);
	for (std::vector<std::string>::iterator it = assemblies.begin(); it != assemblies.end(); it++)
	{
		if (App->file_system->GetFileExtension(*it) == ".dll")
		{
			compile_command += "/reference:" + folder + App->file_system->GetFileName(*it) + " ";
		}
	}
	return system(compile_command.c_str());
}

CSScript* ModuleScriptImporter::DumpAssemblyInfo(MonoAssembly * assembly)
{
	mono_image = mono_assembly_get_image(assembly);
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
