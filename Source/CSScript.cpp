#include "CSScript.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleFileSystem.h"

#pragma comment (lib, "mono/lib/mono-2.0-sgen.lib")


CSScript::CSScript()
{
	domain = nullptr;
	assembly = nullptr;
	mono_class = nullptr;
}

CSScript::~CSScript()
{
}

bool CSScript::InitScript(const char * code, GameObject * container)
{
	mono_install_assembly_preload_hook(assembly_preload_hook, NULL);
	//mono_class = mono_class_from_name()
	return false;
}

void CSScript::StartScript()
{
	MonoMethod* start = GetFunction("Start", 0);

	if (start != nullptr)
	{
		CallFunction(start, nullptr);
	}
}

void CSScript::UpdateScript(float deltaTime)
{
	MonoMethod* update = GetFunction("Update", 0);

	if (update != nullptr)
	{
		CallFunction(update, nullptr);
	}
}

void CSScript::OnCollisionEnter()
{
	MonoMethod* on_collision_enter = GetFunction("OnCollisionEnter", 1);

	if (on_collision_enter != nullptr)
	{
		CallFunction(on_collision_enter, nullptr); //nullptr should be the collision
	}
}

void CSScript::OnCollisionStay()
{
	MonoMethod* on_collision_stay = GetFunction("OnCollisionStay", 1);

	if (on_collision_stay != nullptr)
	{
		CallFunction(on_collision_stay, nullptr); //nullptr should be the collision
	}
}

void CSScript::OnCollisionExit()
{
	MonoMethod* on_collision_exit = GetFunction("OnCollisionExit", 1);

	if (on_collision_exit != nullptr)
	{
		CallFunction(on_collision_exit, nullptr); //nullptr should be the collision
	}
}

void CSScript::OnEnable()
{
	MonoMethod* on_enable = GetFunction("OnEnable", 0);

	if (on_enable != nullptr)
	{
		CallFunction(on_enable, nullptr);
	}
}

void CSScript::OnDisable()
{
	MonoMethod* on_disable = GetFunction("OnDisable", 0);

	if (on_disable != nullptr)
	{
		CallFunction(on_disable, nullptr);
	}
}

void CSScript::SetIntProperty(const char * propertyName, int value)
{
	SetFloatProperty(propertyName, (float)value);
}

int CSScript::GetIntProperty(const char * propertyName)
{
	int value = 0;

	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		mono_field_get_value(object, field, &value);
	}

	return value;
}

void CSScript::SetDoubleProperty(const char * propertyName, double value)
{
	SetFloatProperty(propertyName, (double)value);
}

double CSScript::GetDoubleProperty(const char * propertyName)
{
	double value = 0.0;

	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		mono_field_get_value(object, field, &value);
	}

	return value;
}

void CSScript::SetFloatProperty(const char * propertyName, float value)
{
	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);
	
	if(field)
	{
		void* params = &value;
		mono_field_set_value(object, field, params);
	}
}

float CSScript::GetFloatProperty(const char * propertyName)
{
	float value = 0.0f;

	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		mono_field_get_value(object, field, &value);
	}

	return value;
}

void CSScript::SetBoolProperty(const char * propertyName, bool value)
{
}

bool CSScript::GetBoolProperty(const char * propertyName)
{
	bool value = true;

	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		mono_field_get_value(object, field, &value);
	}

	return value;
}

void CSScript::SetStringProperty(const char * propertyName, const char * value)
{
	
}

std::string CSScript::GetStringProperty(const char * propertyName)
{
	std::string value = "";

	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		mono_field_get_value(object, field, &value);
	}

	return value;
}

void CSScript::SetGameObjectProperty(const char * propertyName, GameObject * value)
{
	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);
	
	if (field)
	{
		MonoClass* gameobject = mono_type_get_class(mono_field_get_type(field));
		MonoObject* new_object = mono_object_new(domain, gameobject);
		MonoClassField* this_field = mono_class_get_field_from_name(gameobject, "this");
		
		if (this_field)
		{
			void* params = value;
			mono_field_set_value(new_object, this_field, params);

			mono_field_set_value(object, field, new_object);
		}
	}
}

GameObject * CSScript::GetGameObjectProperty(const char * propertyName)
{
	/*GameObject* gameobject = nullptr;

	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		MonoObject* mono_object = nullptr;
		mono_field_get_value(object, field, &mono_object);
		
		if (mono_object != nullptr)
		{
			MonoClass* object_class = mono_object_get_class(mono_object);
			field = mono_class_get_field_from_name(object_class, "this");
			
			if (field)
			{
				mono_field_get_value(mono_object, field, &gameobject);
			}
		}
	}

	return gameobject;*/

	GameObject* value = nullptr;
	MonoClassField* Field = mono_class_get_field_from_name(mono_class, propertyName);
	if (Field)
	{
		mono_field_get_value(object, Field, &value);
	}

	return value;
}

void CSScript::SetVec2Property(const char * propertyName, float2 value)
{
}

float2 CSScript::GetVec2Property(const char * propertyName)
{
	float2 value;

	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		mono_field_get_value(object, field, &value);
	}

	return value;
}

void CSScript::SetVec3Property(const char * propertyName, float3 value)
{
}

float3 CSScript::GetVec3Property(const char * propertyName)
{
	float3 value;

	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		mono_field_get_value(object, field, &value);
	}

	return value;
}

void CSScript::SetVec4Property(const char * propertyName, float4 value)
{
}

float4 CSScript::GetVec4Property(const char * propertyName)
{
	float4 value;

	MonoClassField* field = mono_class_get_field_from_name(mono_class, propertyName);

	if (field)
	{
		mono_field_get_value(object, field, &value);
	}

	return value;
}

void CSScript::SetTextureProperty(const char * propertyName, Texture * value)
{
}

Texture * CSScript::GetTextureProperty(const char * propertyName)
{
	Texture* value = nullptr;
	MonoClassField* Field = mono_class_get_field_from_name(mono_class, propertyName);
	if (Field)
	{
		mono_field_get_value(object, Field, &value);
	}

	return value;
}

std::vector<ScriptField*> CSScript::GetScriptFields()
{
	return std::vector<ScriptField*>();
}

MonoMethod * CSScript::GetFunction(const char * functionName, int parameters_count)
{
	MonoMethod* method = nullptr;

	if (mono_class != nullptr)
	{
		method = mono_class_get_method_from_name(mono_class, functionName, parameters_count);
	}

	return method;
}

void CSScript::CallFunction(MonoMethod * function, void ** parameter)
{
	MonoObject* exception = nullptr;
	mono_runtime_invoke((MonoMethod*)function, object, parameter, &exception);

	if (exception)
	{
		mono_print_unhandled_exception(exception);
	}
}

 static MonoAssembly * assembly_preload_hook(MonoAssemblyName * assembly_name, char ** assemblies_path, void * user_data)
{

	 std::string name(mono_assembly_name_get_name(assembly_name));

	 if (App->file_system->GetFileExtension(name) != ".dll")
	 {
		 name += ".dll";
	 }

	 MonoAssembly* assembly = mono_assembly_loaded(assembly_name);

	 if (assembly)
	 {
		 CONSOLE_DEBUG("Loaded Assembly '%s'.", name);
		 return assembly;
	 }

	return nullptr;
}
